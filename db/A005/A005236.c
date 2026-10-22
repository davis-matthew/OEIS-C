/*
 * A005236  Barriers for omega(n): numbers n such that, for all m < n, m +
 *          omega(m) <= n.
 * https://oeis.org/A005236
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   sieve search: a(n) is the n-th m >= 2 with max_{k<m} (k +
 *            omega(k)) <= m; omega comes from a segmented sieve
 * Range:     u8 n <= 48, u16 n <= 896, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define SEG 65536

static uint64_t *bar = 0;
static size_t nbar = 0, capbar = 0;
static uint64_t next_m = 1;
static uint64_t maxv = 0;

static uint32_t *primes = 0;
static size_t np = 0;
static uint64_t plimit = 0;

static void ensure_primes(uint64_t lim) {
    uint64_t newlim, i, j;
    unsigned char *s;
    if (plimit >= lim) return;
    newlim = lim * 2;
    if (newlim < 1024) newlim = 1024;
    s = (unsigned char *)calloc((size_t)newlim + 1, 1);
    free(primes);
    primes = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)newlim / 2 + 2));
    np = 0;
    for (i = 2; i <= newlim; i++) {
        if (!s[i]) {
            primes[np++] = (uint32_t)i;
            for (j = i * i; j <= newlim; j += i) s[j] = 1;
        }
    }
    free(s);
    plimit = newlim;
}

static void process_segment(void) {
    static uint64_t rem[SEG];
    static unsigned char cnt[SEG];
    uint64_t lo = next_m, hi = lo + SEG, r = 1;
    size_t i, k;
    while ((r + 1) * (r + 1) <= hi - 1) r++;
    ensure_primes(r);
    for (i = 0; i < SEG; i++) {
        rem[i] = lo + i;
        cnt[i] = 0;
    }
    for (k = 0; k < np && (uint64_t)primes[k] <= r; k++) {
        uint64_t p = primes[k];
        uint64_t st = ((lo + p - 1) / p) * p, j;
        for (j = st; j < hi; j += p) {
            size_t idx = (size_t)(j - lo);
            cnt[idx]++;
            while (rem[idx] % p == 0) rem[idx] /= p;
        }
    }
    for (i = 0; i < SEG; i++) {
        uint64_t m = lo + i;
        uint64_t om = cnt[i] + (rem[i] > 1 ? 1 : 0);
        if (m >= 2 && maxv <= m) {
            if (nbar == capbar) {
                capbar = capbar ? capbar * 2 : 1024;
                bar = (uint64_t *)realloc(bar, sizeof(uint64_t) * capbar);
            }
            bar[nbar++] = m;
        }
        if (m + om > maxv) maxv = m + om;
    }
    next_m = hi;
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    while (nbar < (size_t)n) process_segment();
    return (seq_t)bar[n - 1];
}

/*
 * A076542  Numbers n such that A068340(n) = +-1.
 * https://oeis.org/A076542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  hard, nonn, more
 * Formula:   search -- sieve mu(k) in segments, keep the running sum
 *            A068340(k) = Sum_{j=1..k} mu(j)*j, and record every k where that
 *            sum is +1 or -1 (k with mu(k) = 0 are included too, because the
 *            sum stays the same)
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Validated: partial, pass, 8 terms through n = 8 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A076542_SEG (1u << 18)
#define A076542_LIMIT 200000000ULL

static uint64_t *A076542_found = NULL;
static size_t A076542_cnt = 0, A076542_cap = 0;
static uint64_t A076542_scanned = 0;
static int64_t A076542_sum = 0;
static uint32_t *A076542_pr = NULL;
static size_t A076542_npr = 0;
static uint64_t A076542_plim = 0;

static void A076542_primes_upto(uint64_t lim)
{
    uint8_t *s = (uint8_t *)malloc((size_t)lim + 1);
    size_t c = 0;
    memset(s, 1, (size_t)lim + 1);
    for (uint64_t i = 2; i <= lim; ++i) {
        if (s[i]) {
            c++;
            for (uint64_t j = i * i; j <= lim; j += i) s[j] = 0;
        }
    }
    free(A076542_pr);
    A076542_pr = (uint32_t *)malloc(sizeof(uint32_t) * (c + 1));
    A076542_npr = 0;
    for (uint64_t i = 2; i <= lim; ++i)
        if (s[i]) A076542_pr[A076542_npr++] = (uint32_t)i;
    free(s);
    A076542_plim = lim;
}

static void A076542_segment(void)
{
    uint64_t lo = A076542_scanned + 1;
    uint64_t hi = lo + A076542_SEG - 1;
    uint64_t need = 1;
    while (need * need <= hi) need++;
    if (need > A076542_plim) A076542_primes_upto(need * 2 + 100);

    int8_t *mu = (int8_t *)malloc(A076542_SEG);
    uint64_t *rem = (uint64_t *)malloc(sizeof(uint64_t) * A076542_SEG);
    for (uint32_t i = 0; i < A076542_SEG; ++i) {
        mu[i] = 1;
        rem[i] = lo + i;
    }
    for (size_t k = 0; k < A076542_npr; ++k) {
        uint64_t p = A076542_pr[k];
        if (p * p > hi) break;
        uint64_t st = ((lo + p - 1) / p) * p;
        for (uint64_t j = st; j <= hi; j += p) {
            mu[j - lo] = (int8_t)-mu[j - lo];
            rem[j - lo] /= p;
        }
        uint64_t p2 = p * p;
        st = ((lo + p2 - 1) / p2) * p2;
        for (uint64_t j = st; j <= hi; j += p2) mu[j - lo] = 0;
    }
    for (uint32_t i = 0; i < A076542_SEG; ++i) {
        int m = mu[i];
        if (m && rem[i] > 1) m = -m;
        if (m) A076542_sum += (int64_t)m * (int64_t)(lo + i);
        if (A076542_sum == 1 || A076542_sum == -1) {
            if (A076542_cnt == A076542_cap) {
                A076542_cap = A076542_cap ? A076542_cap * 2 : 64;
                A076542_found = (uint64_t *)realloc(A076542_found, sizeof(uint64_t) * A076542_cap);
            }
            A076542_found[A076542_cnt++] = lo + i;
        }
    }
    free(mu);
    free(rem);
    A076542_scanned = hi;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    while (A076542_cnt < (size_t)n && A076542_scanned < A076542_LIMIT)
        A076542_segment();
    if (A076542_cnt < (size_t)n) return (seq_t)0;
    return (seq_t)A076542_found[n - 1];
}

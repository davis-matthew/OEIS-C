/*
 * A001494  Numbers k such that phi(k) = phi(k+2).
 * https://oeis.org/A001494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search: a(n) = n-th k with phi(k) = phi(k+2); phi computed by a
 *            segmented totient sieve
 * Range:     u8 n <= 10, u16 n <= 320, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define A1494_B 65536

static uint64_t *a1494_terms = NULL;
static size_t a1494_cnt = 0, a1494_cap = 0;
static uint64_t a1494_next = 1;

static uint32_t *a1494_primes = NULL;
static size_t a1494_np = 0;
static uint64_t a1494_plim = 0;

static void a1494_gen_primes(uint64_t lim) {
    char *c = (char *)malloc((size_t)lim + 1);
    uint64_t i, j;
    memset(c, 1, (size_t)lim + 1);
    c[0] = 0;
    if (lim >= 1) c[1] = 0;
    for (i = 2; i * i <= lim; i++)
        if (c[i])
            for (j = i * i; j <= lim; j += i) c[j] = 0;
    free(a1494_primes);
    a1494_np = 0;
    for (i = 2; i <= lim; i++) if (c[i]) a1494_np++;
    a1494_primes = (uint32_t *)malloc((a1494_np + 1) * sizeof(uint32_t));
    a1494_np = 0;
    for (i = 2; i <= lim; i++) if (c[i]) a1494_primes[a1494_np++] = (uint32_t)i;
    free(c);
    a1494_plim = lim;
}

static void a1494_push(uint64_t k) {
    if (a1494_cnt == a1494_cap) {
        size_t nc = a1494_cap ? a1494_cap * 2 : 1024;
        a1494_terms = (uint64_t *)realloc(a1494_terms, nc * sizeof(uint64_t));
        a1494_cap = nc;
    }
    a1494_terms[a1494_cnt++] = k;
}

static void a1494_block(void) {
    static uint64_t rem[A1494_B + 2], ph[A1494_B + 2];
    uint64_t lo = a1494_next, len = A1494_B + 2, hi = lo + len - 1;
    uint64_t i;
    size_t t;
    if (a1494_plim * a1494_plim < hi) {
        uint64_t s = 1;
        while (s * s < hi) s++;
        a1494_gen_primes(2 * s + 1000);
    }
    for (i = 0; i < len; i++) { rem[i] = lo + i; ph[i] = lo + i; }
    for (t = 0; t < a1494_np; t++) {
        uint64_t p = a1494_primes[t], m;
        if (p * p > hi) break;
        m = ((lo + p - 1) / p) * p;
        for (; m <= hi; m += p) {
            uint64_t idx = m - lo;
            ph[idx] -= ph[idx] / p;
            do { rem[idx] /= p; } while (rem[idx] % p == 0);
        }
    }
    for (i = 0; i < len; i++)
        if (rem[i] > 1) ph[i] -= ph[i] / rem[i];
    for (i = 0; i < A1494_B; i++)
        if (ph[i] == ph[i + 2]) a1494_push(lo + i);
    a1494_next = lo + A1494_B;
}

seq_t a(idx_t n) {
    while (a1494_cnt < n) a1494_block();
    return (seq_t)a1494_terms[n - 1];
}

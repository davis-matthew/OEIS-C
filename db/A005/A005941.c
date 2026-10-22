/*
 * A005941  Inverse of the Doudna sequence A005940.
 * https://oeis.org/A005941
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + A156552(n); if n =
 *            prime(i_1)*prime(i_2)*...*prime(i_k) with i_1 <= i_2 <= ... <=
 *            i_k, then A156552(n) = Sum_{j=1..k} 2^(i_j - 1 + j - 1). Prime
 *            indices are found by factoring n with a cached sieve
 * Range:     u8 n <= 22, u16 n <= 58, u32 n <= 136, u64 n <= 312,
 *            u128 n <= 726, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint32_t *pi_tab = NULL;   /* pi_tab[m] = number of primes <= m */
static uint64_t pi_cap = 0;

static void ensure_sieve(uint64_t lim) {
    if (lim < pi_cap) return;
    uint64_t cap = pi_cap ? pi_cap : 1024;
    while (cap <= lim) cap *= 2;
    unsigned char *comp = (unsigned char *)calloc(cap, 1);
    uint32_t *tab = (uint32_t *)malloc(cap * sizeof(uint32_t));
    comp[0] = 1;
    if (cap > 1) comp[1] = 1;
    for (uint64_t i = 2; i * i < cap; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j < cap; j += i) comp[j] = 1;
    uint32_t c = 0;
    for (uint64_t i = 0; i < cap; i++) {
        if (!comp[i]) c++;
        tab[i] = c;
    }
    free(comp);
    free(pi_tab);
    pi_tab = tab;
    pi_cap = cap;
}

static seq_t pow2_seq(uint64_t e) {
    seq_t r = 1;
    for (uint64_t i = 0; i < e; i++) r = r * 2;
    return r;
}

seq_t a(idx_t n) {
    ensure_sieve(n);
    seq_t s = 0;
    uint64_t m = n;
    uint64_t j = 0; /* number of prime factors taken so far */
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p != 0) continue;
        uint64_t idx = pi_tab[p];
        while (m % p == 0) {
            m /= p;
            s += pow2_seq(idx - 1 + j);
            j++;
        }
    }
    if (m > 1) {
        uint64_t idx = pi_tab[m];
        s += pow2_seq(idx - 1 + j);
    }
    return s + 1;
}

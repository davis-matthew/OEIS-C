/*
 * A069252  Number of basis partitions of n+81 with Durfee square size 9.
 * https://oeis.org/A069252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = N[n] – Σ_{k=1}^{31} D[k]·a(n-k) (with D[0] = 1)
 * Range:     u8 n <= 10, u16 n <= 29, u32 n <= 135, u64 n <= 2188,
 *            u128 n <= 10000, big any n
 * Helpers:   binom9, init_polys, mult_poly
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The coefficients D[0..31] and N[0..31] come from the rational
   generating function listed in the OEIS entry. */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/binom9.h"
#include "../utils/init_polys.h"
#include "../utils/mult_poly.h"

/* Sequence function */
seq_t a(idx_t n)
{
    /* static state */
    static int initialized = 0;
    static long long D[32];
    static long long N[32];
    if (!initialized) {
        init_polys(D, N);
        initialized = 1;
    }

    /* dynamic cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    if (cache_len <= (size_t)n) {
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t sum = (i <= 31 ? (seq_t)N[i] : (seq_t)0);
            for (int k = 1; k <= 31; ++k) {
                if (i >= (size_t)k)
                    sum -= (seq_t)D[k] * cache[i - k];
            }
            cache[i] = sum;
        }
        cache_len = n + 1;
    }

    return cache[(size_t)n];
}

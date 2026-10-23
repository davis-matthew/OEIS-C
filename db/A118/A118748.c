/*
 * A118748  a(n) = Product_{k=1..n} P(k), where P(k) is the smallest prime >=
 *          2*k.
 * https://oeis.org/A118748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Product_{k=1..n} P(k) where P(k) is the smallest prime >=
 *            2*k (offset 1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   is_prime, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* malloc, realloc */
#include <stdint.h>
#include <stdbool.h>

/* Provided utility functions */
#include "../utils/is_prime.h"   /* bool is_prime(idx_t x); */
#include "../utils/seq_mul.h"    /* seq_t seq_mul(seq_t a, seq_t b); */

seq_t a(idx_t n)
{
    /* Static cache of already computed values */
    static seq_t *cache = NULL;   /* array of a(1), a(2), ... */
    static size_t cap   = 0;     /* allocated capacity of cache */
    static idx_t  last  = 0;     /* largest index computed so far (0-based) */

    if (n == (idx_t)0)          /* not used by OEIS, safety fallback */
        return (seq_t)0;

    /* Ensure the cache has enough space for n elements */
    if (n > (idx_t)cap) {
        size_t new_cap = (size_t)n;
        /* realloc may return NULL on failure; let caller handle */
        cache = (seq_t *)realloc((void *)cache, new_cap * sizeof(seq_t));
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = last + (idx_t)1; i <= n; ++i) {
        /* Find smallest prime >= 2*i */
        idx_t candidate = (idx_t)2 * i;
        while (!is_prime(candidate))
            ++candidate;

        seq_t prime = (seq_t)candidate;

        if (i == (idx_t)1) {
            cache[0] = prime;                      /* a(1) = P(1) */
        } else {
            cache[(size_t)(i - (idx_t)1)] = seq_mul(cache[(size_t)(i - (idx_t)2)], prime);
        }
    }

    last = n;   /* cache now contains at least n elements */
    return cache[(size_t)(n - (idx_t)1)];
}

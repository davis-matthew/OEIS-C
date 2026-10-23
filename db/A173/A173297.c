/*
 * A173297  Numbers k such that exactly one of k^2 + k - 1 and k^2 + k + 1 is
 *          prime.
 * https://oeis.org/A173297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   numbers k such that exactly one of k^2 + k - 1 and k^2 + k + 1
 *            is prime
 * Range:     u8 n <= 9, u16 n <= 407, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/isPrimeGeneric.h"   /* bool isPrimeGeneric(seq_t n); */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* found k values */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_len = 0;     /* number of values currently stored */
    static idx_t  search_k = 1;      /* next k candidate to test */

    /* Grow cache until it contains at least n entries */
    while (cache_len < (size_t)n) {
        /* Compute the two numbers to test for primality */
        seq_t m  = (seq_t)search_k * ((seq_t)search_k + 1); /* k*(k+1) */
        seq_t m_minus = m - 1;                              /* k^2 + k - 1 */
        seq_t m_plus  = m + 1;                              /* k^2 + k + 1 */

        bool p_minus = isPrimeGeneric(m_minus);
        bool p_plus  = isPrimeGeneric(m_plus);

        if (p_minus ^ p_plus) {            /* exactly one is prime */
            /* Allocate or grow cache if needed */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) { /* allocation failed – stop gracefully */
                    return (seq_t)0;
                }
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)search_k;
        }
        ++search_k;                         /* next candidate k */
    }

    return cache[n - 1];                     /* 1‑based indexing */
}

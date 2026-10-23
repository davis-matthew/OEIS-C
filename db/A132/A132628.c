/*
 * A132628  Numbers k for which the sum of their digits divides sigma(k).
 * https://oeis.org/A132628
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers k such that sum of digits of k divides σ(k) */
#include <stdlib.h>
#include <stddef.h>
/* Include utility headers */
#include "../utils/digit_sum.h"
#include "../utils/sigma_of.h"

/* Computes the n-th term of sequence A132628.
 * The sequence elements are stored in a static cache so calls may be made
 * in any order.
 */
seq_t a(idx_t n)
{
    /* Static cache of found numbers */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;     /* number of stored terms */
    static size_t cache_cap = 0;     /* allocated capacity of cache */
    static idx_t next_k = 1;         /* next integer to test */

    /* Lazily allocate the cache */
    if (cache == NULL) {
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* Extend the cache until we have at least n terms */
    while (cache_len < (size_t)n) {
        seq_t ds = digit_sum((seq_t)next_k);
        if (ds != (seq_t)0) {
            seq_t sigma = sigma_of(next_k);
            if ((sigma % ds) == (seq_t)0) {
                /* Store the term */
                if (cache_len == cache_cap) {
                    cache_cap <<= 1;
                    cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)next_k;
            }
        }
        ++next_k;
    }

    return cache[n - 1];
}

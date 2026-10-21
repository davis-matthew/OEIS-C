/*
 * A085781  a(n) = 2*binomial(2*n+1,n+1) - 2^n.
 * https://oeis.org/A085781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(2*n+1, n+1) - 2^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes needed utility functions */
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

/* Static cache for previously computed terms */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* Grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = n + 1;
        /* Reallocate cache array */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* Compute terms not yet cached */
        for (idx_t i = cache_size; i < new_size; ++i) {
            seq_t bin = binomial(2 * i + 1, i + 1);   /* binomial(2n+1, n+1) */
            seq_t term = 2 * bin;                     /* 2 * binomial(...) */
            seq_t pow  = pow2(i);                     /* 2^i */
            cache[i] = term - pow;                    /* a(i) = 2*bin - 2^i */
        }
        cache_size = new_size;
    }
    return cache[n];
}

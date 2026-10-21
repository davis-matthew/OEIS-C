/*
 * A218516  Number of n X 2 arrays of the minimum value of corresponding
 *          elements and their horizontal or antidiagonal neighbors in a
 *          random 0..3 n X 2 array.
 * https://oeis.org/A218516
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n-1) + 3*a(n-2) - 4*a(n-4) + a(n-5) Initial values
 *            (offset 1): a(1)=4, a(2)=36, a(3)=236, a(4)=1528, a(5)=9858
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 47,
 *            big any n
 * Helpers:   multiply_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/multiply_small.h"

/* memoisation array (zero‑based: cache[0] = a(1)) */
static seq_t *cache = NULL;
static idx_t cache_size = 0;

static const seq_t init_terms[5] = {4, 36, 236, 1528, 9858};

seq_t a(idx_t n)
{
    if (n < 1) return 0;                /* undefined for n < 1 */

    if (n <= 5) return init_terms[n - 1];

    /* Expand cache to contain at least n terms. */
    if (cache_size < n) {
        size_t old_size = cache_size;
        size_t new_size = (size_t)n;

        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;

        /* initialise the first five terms if not already done */
        if (old_size < 5) {
            size_t upto = (new_size < 5 ? new_size : 5);
            for (size_t i = old_size; i < upto; ++i) {
                cache[i] = init_terms[i];
            }
        }

        /* compute remaining terms */
        size_t start = (old_size > 5) ? old_size : 5;
        for (size_t j = start; j < new_size; ++j) {
            seq_t val = multiply_small(cache[j - 1], 6) /* 6*a(n-1) */
                      + multiply_small(cache[j - 2], 3) /* 3*a(n-2) */
                      - multiply_small(cache[j - 4], 4) /* 4*a(n-4) */
                      + cache[j - 5];                 /* + a(n-5) */
            cache[j] = val;
        }

        cache_size = new_size;
    }

    return cache[n - 1];
}

/*
 * A069559  Triangular numbers in which the k-th significant digit either
 *          divides k or is a multiple of k.
 * https://oeis.org/A069559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 32, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   satisfies_triangular
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A069559: Triangular numbers in which the k-th significant digit
 * either divides k or is (a positive) multiple of k.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/satisfies_triangular.h"

/* Return the n-th term of the sequence.  n is 1‑based.  The
 * function caches all computed terms so that arbitrary call
 * order is supported efficiently.
 */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* undefined, return zero */

    /* static cache for computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_m = 0;           /* last triangular index examined */

    /* Return if already computed */
    if (cache_len >= n)
        return cache[n - 1];

    /* Grow the cache to the required capacity */
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap < cache_len + 1)
            new_cap <<= 1;
        seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Generate triangular numbers until we have enough terms */
    while (cache_len < n) {
        ++last_m;
        seq_t T = (seq_t)last_m * ((seq_t)last_m + 1) / 2;
        if (satisfies_triangular(T))
            cache[cache_len++] = T;
    }
    return cache[n - 1];
}

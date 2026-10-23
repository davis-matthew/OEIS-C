/*
 * A125422  Number of base 10 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑10 circular n‑digit numbers with each pair
 *            of adjacent digits differing by at most 7. This equals trace(Aⁿ)
 *            where A is the 10×10 adjacency matrix A[i][j] = 1 if |i−j|≤7
 *            else 0. The sequence satisfies the linear recurrence a(n) =
 *            10·a(n−1) − 3·a(n−2) − 20·a(n−3) + a(n−4) + 6·...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, realloc */

static seq_t *cache = NULL;    /* memoisation array */
static idx_t  cache_len = 0;   /* current number of stored values */
static idx_t  cache_cap = 0;   /* current allocated capacity */

seq_t a(idx_t n)
{
    /* initialise the cache with the first six exact values */
    if (cache_len == 0) {
        cache_cap = 8;                     /* initial capacity > 6 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        cache[2] = (seq_t)94;
        cache[3] = (seq_t)850;
        cache[4] = (seq_t)8022;
        cache[5] = (seq_t)75830;
        cache_len = 6;
    }

    /* return a value already calculated */
    if (n < cache_len)
        return cache[n];

    /* extend the cache up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) val = (seq_t)1;
        else if (i == 1) val = (seq_t)10;
        else if (i == 2) val = (seq_t)94;
        else if (i == 3) val = (seq_t)850;
        else if (i == 4) val = (seq_t)8022;
        else if (i == 5) val = (seq_t)75830;
        else
            val = 10 * cache[i - 1]
                - 3  * cache[i - 2]
                - 20 * cache[i - 3]
                +     cache[i - 4]
                + 6  * cache[i - 5];

        /* grow the cache if necessary */
        if (i >= cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }
        cache[i] = val;
    }
    cache_len = n + 1;

    return cache[n];
}

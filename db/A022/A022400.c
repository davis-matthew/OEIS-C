/*
 * A022400  Fibonacci sequence beginning 1, 30.
 * https://oeis.org/A022400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A022400: Fibonacci variant
   a(0) = 1, a(1) = 30, a(n) = a(n-1) + a(n-2) for n >= 2.  */
#include <stdlib.h>

/* Cache of computed values – external linkage */
static seq_t *cache = NULL;   /* pointer to array of seq_t values          */
static idx_t cache_cap = 0;   /* number of elements currently stored      */

/* Return the nth term of the sequence.  */
seq_t a(idx_t n)
{
    /* At least two slots are needed for the seed values. */
    if (cache_cap < 2) {
        idx_t init_cap = (n + 1 > 2 ? n + 1 : 2);   /* ensure capacity up to n */
        seq_t *tmp = (seq_t *)realloc(cache, init_cap * sizeof(seq_t));
        cache = tmp;                                         /* may be NULL (error) */
        cache_cap = init_cap;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)30;
    }

    /* Grow the cache if the requested index is beyond the current capacity. */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;                                         /* may be NULL (error) */
        cache_cap = new_cap;
    }

    /* Compute any missing terms.  The loop is harmless if the terms are already
       present because it will recompute the same values.  */
    for (idx_t i = 2; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2];
    }

    return cache[n];
}

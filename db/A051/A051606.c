/*
 * A051606  a(n) = (3*n+6)!!!/6!!!, related to A032031 ((3*n)!!! triple
 *          factorials).
 * https://oeis.org/A051606
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1; for n ≥ 1: a(n) = a(n-1) * (3*n + 6)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoised values      */
    static idx_t  cache_len = 0;     /* number of values stored */

    /* Initialise cache with a(0) */
    if (!cache) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        cache[0] = (seq_t)1;          /* a(0) = 1 */
        cache_len = 1;
    }

    /* Extend the cache up to the requested index */
    while (cache_len <= n) {
        idx_t i = cache_len;          /* index to compute (current n) */
        /* grow the array by one element */
        cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));

        /* multiply by 3*i + 6  (since i = current n) */
        seq_t mult = (seq_t)3 * (seq_t)(i + 2);   /* 3*i + 6 */
        cache[i] = cache[i-1] * mult;

        ++cache_len;                  /* one more value now stored */
    }

    return cache[n];
}

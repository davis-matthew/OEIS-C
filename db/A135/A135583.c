/*
 * A135583  a(n) = 4*a(n-1) - 4 for n>0, a(0)=3.
 * https://oeis.org/A135583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - 4, a(0) = 3
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Memoised computation to allow arbitrary order of calls. */
    static seq_t *cache = NULL;     /* array of known terms   */
    static idx_t  cache_len = 0;    /* number of terms stored */

    /* Initialise for the first call. */
    if (cache_len == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t));  /* space for a(0) */
        cache[0] = (seq_t)3;                   /* a(0) = 3       */
        cache_len = 1;
    }

    /* Extend the cache up to n if necessary. */
    if (cache_len <= n) {
        idx_t old_len = cache_len;
        cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        for (idx_t i = old_len; i <= n; ++i) {
            cache[i] = cache[i - 1] * (seq_t)4 - (seq_t)4;
        }
        cache_len = n + 1;
    }

    return cache[n];
}

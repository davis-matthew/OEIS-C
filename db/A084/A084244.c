/*
 * A084244  a(0)=1, a(1)=5, a(n) = -3*a(n-1), n>1.
 * https://oeis.org/A084244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(0) = 1; a(1) = 5; a(n) = -3*a(n-1) for n > 1
 * Range:     i8 n <= 3, i16 n <= 8, i32 n <= 19, i64 n <= 39, i128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization of computed terms */
static idx_t  cache_len = 0;
static seq_t *cache      = NULL;

seq_t a(idx_t n)
{
    /* handle base cases directly */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)5;

    /* initialize cache on first use */
    if (cache == NULL) {
        cache_len = 2;                     /* 0 and 1 already stored */
        cache     = (seq_t*)malloc(sizeof(seq_t) * cache_len);
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)5;
    }

    /* extend cache if needed */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        cache_len = n + 1;
        cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_len);
        for (idx_t i = old_len; i <= n; ++i) {
            /* recurrence: a(i) = -3 * a(i-1) */
            cache[i] = - ((seq_t)3 * cache[i - 1]);
        }
    }

    return cache[n];
}

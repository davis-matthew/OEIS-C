/*
 * A146983  a(n) = A002531(n)*A002531(n+1).
 * https://oeis.org/A146983
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 3*(-1)^n with a(0)=1, a(1)=2.
 *            Sequence A146983 is a(n) = A002531(n) * A002531(n+1)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoisation.  The cache is grown on demand so
   the function works correctly for any order of calls. */
static idx_t    cache_len = 0;   /* number of elements currently stored */
static seq_t   *cache     = NULL;/* pointer to the cache array */

/* Compute a(n) */
seq_t a(idx_t n)
{
    /* Initialise the cache with the first two terms on the first call. */
    if (cache_len == 0) {
        cache_len = 2;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_len);
        if (!cache)   /* allocation failed – return 0 as a safe default */
            return (seq_t)0;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
    }

    /* Grow the cache if the requested index is past the end. */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_len);
        if (!tmp)   /* allocation failed – keep old cache and return 0 */
            return (seq_t)0;
        cache = tmp;

        /* Fill the missing values using the recurrence. */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t prev  = cache[i - 1];
            seq_t prev2 = cache[i - 2];
            seq_t val   = prev * (seq_t)4 - prev2;   /* 4*a(i-1) - a(i-2) */
            if ((i & 1) == 0)                       /* even i : +3 */
                val += (seq_t)3;
            else                                    /* odd i  : -3 */
                val -= (seq_t)3;
            cache[i] = val;
        }
        cache_len = new_len;
    }

    return cache[n];
}

/*
 * A144534  Denominators of continued fraction convergents to sqrt(8/9).
 * https://oeis.org/A144534
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, a(1)=1; for n >= 2
   if n is even: a(n) = 16*a(n-1) + a(n-2)
   if n is odd : a(n) = 2*a(n-1)  + a(n-2) */

#include <stdlib.h>

/* Cache of previously computed terms */
static seq_t *cache = NULL;
static idx_t  cache_cap = 0;
static idx_t  max_computed = 0;

/* Ensure the cache can hold indices up to 'need' */
static void
ensure_cache_size(idx_t need)
{
    if (need + 1 > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap : 2;
        while (new_cap <= need)
            new_cap <<= 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            exit(1);
        cache      = new_cache;
        cache_cap  = new_cap;
    }
}

/* Compute a(n) with memoisation */
seq_t
a(idx_t n)
{
    if (cache == NULL) {                /* initialise cache   */
        cache_cap = 2;
        cache     = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache)
            exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        max_computed = 1;
    }

    if (n > max_computed) {
        ensure_cache_size(n);
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            if ((i & 1) == 0)            /* even index  */
                cache[i] = (seq_t)16 * cache[i-1] + cache[i-2];
            else                         /* odd index   */
                cache[i] = (seq_t)2  * cache[i-1] + cache[i-2];
        }
        max_computed = n;
    }
    return cache[n];
}

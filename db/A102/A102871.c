/*
 * A102871  a(n) = a(n-3) - 5*a(n-2) + 5*a(n-1), a(0) = 1, a(1) = 3, a(2) =
 *          10.
 * https://oeis.org/A102871
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-3) - 5*a(n-2) + 5*a(n-1), a(0)=1, a(1)=3, a(2)=10
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Cache for previously computed values.
   cache[i] stores a(i) for i < cached_upto.*/
static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cached_upto = 0;

seq_t a(idx_t n)
{
    /* Initialise the cache with the first three terms if not done yet. */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t*)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) {
            /* Allocation failed – return 0 as a fallback (should not happen). */
            return (seq_t)0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)10;
        cached_upto = 3;   /* next index to compute */
    }

    /* Return cached value if already computed. */
    if (n < cached_upto) {
        return cache[n];
    }

    /* Ensure enough space in the cache. */
    if ((idx_t)(n + 1) > cache_size) {
        idx_t newsize = cache_size * 2;
        if (newsize < n + 1) newsize = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newsize * sizeof(seq_t));
        if (tmp == NULL) {
            /* Realloc failed – return 0 as a fallback. */
            return (seq_t)0;
        }
        cache = tmp;
        cache_size = newsize;
    }

    /* Iteratively compute a(i) for i = cached_upto ... n.  The recursion
       uses only the last three values, so we can calculate each term in
       constant time.  An intermediate value might be negative in signed
       arithmetic; using unsigned arithmetic with the rearranged formula
       (a(n-3) + 5*(a(n-1)-a(n-2))) avoids underflow. */
    for (idx_t i = cached_upto; i <= n; ++i) {
        seq_t prev3 = cache[i - 3];
        seq_t prev1 = cache[i - 1];
        seq_t prev2 = cache[i - 2];
        seq_t diff  = prev1 - prev2;        /* a(n-1) - a(n-2)   */
        seq_t val   = prev3 + diff * (seq_t)5;  /* a(n-3) + 5*diff  */
        cache[i] = val;
    }

    cached_upto = n + 1;
    return cache[n];
}

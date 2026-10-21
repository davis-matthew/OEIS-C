/*
 * A081190  8th binomial transform of (1,0,1,0,1,.....), A059841.
 * https://oeis.org/A081190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 16*a(n-1) - 63*a(n-2), a(0)=1, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This matches a(n) = (7^n + 9^n)/2.  The implementation uses a
   simple linear recurrence with memoisation.  For any n ≥ 0 it
   returns the exact value when it fits in seq_t, handling larger
   values correctly when seq_t is an arbitrary‑precision class. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation.  cache[i] holds a(i).  */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialise cache with first two terms.  */
    if (cache == NULL) {
        cache_len = 2;
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        cache[0] = (seq_t)1;   /* a(0) */
        cache[1] = (seq_t)8;   /* a(1) */
    }

    /* Return cached value if available.  */
    if (n < cache_len)
        return cache[n];

    /* Grow cache to accommodate up to index n.  */
    idx_t new_len = n + 1;
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    /* Compute missing terms using the recurrence.  */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)16 * cache[i - 1] - (seq_t)63 * cache[i - 2];
    }
    cache_len = new_len;

    return cache[n];
}

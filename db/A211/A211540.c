/*
 * A211540  Number of ordered triples (w,x,y) with all terms in {1..n} and 2w
 *          = 3x + 4y.
 * https://oeis.org/A211540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) - a(n-4) - a(n-5) + a(n-6) with initial
 *            values a(0)=a(1)=a(2)=a(3)=a(4)=0, a(5)=1
 * Range:     u8 n <= 57, u16 n <= 888, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoisation buffer   */
static idx_t  cache_size = 0;   /* number of elements in cache */

/* Ensure that cache holds values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_size <= n) {
        idx_t new_size = n + 1;                     /* desired length   */
        seq_t *tmp = (seq_t *)realloc(cache,
                                       new_size * sizeof(seq_t));
        if (!tmp) abort();                          /* allocation failed */
        cache = tmp;

        /* Compute values for indices [old_size .. n] */
        for (idx_t i = cache_size; i <= n; ++i) {
            if (i < 6) {                            /* initialise small indices   */
                if (i <= 4) cache[i] = (seq_t)0;
                else /* i == 5 */ cache[i] = (seq_t)1;
            } else {                                /* recurrence for i >= 6       */
                cache[i] = cache[i-1] + cache[i-2]
                            - cache[i-4] - cache[i-5]
                            + cache[i-6];
            }
        }
        cache_size = new_size;                      /* update cache size         */
    }
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}

/*
 * A064650  a(n) = floor(a(n-1)/2) + a(n-2) with a(0)=1, a(1)=2.
 * https://oeis.org/A064650
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(a(n-1)/2) + a(n-2), with a(0)=1, a(1)=2
 * Range:     u8 n <= 22, u16 n <= 44, u32 n <= 89, u64 n <= 179,
 *            u128 n <= 358, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>   /* for bool */


/* maximum index that will be cached explicitly */
enum { MAX_CACHE = 20001 };

/* cache of computed values; zero indicates “not yet computed” */
static seq_t cache[MAX_CACHE] = {0};

/* flag that indicates whether the cache has been initialised */
static bool initialized = false;

/* initialise the cache with the two base values and a(2) */
static void init_cache(void)
{
    cache[0] = 1;
    cache[1] = 2;
    cache[2] = cache[1] / 2 + cache[0];
    initialized = true;
}

/* return the n‑th term of A064650 */
seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return 1;
    if (n == 1) return 2;

    /* initialise the cache on the first call */
    if (!initialized)
        init_cache();

    /* use the cache if the requested index is within its bounds */
    if (n < MAX_CACHE) {
        if (cache[n] == 0) {              /* compute all missing terms up to n */
            for (idx_t i = 2; i <= n; ++i) {
                if (cache[i] == 0)
                    cache[i] = cache[i-1] / 2 + cache[i-2];
            }
        }
        return cache[n];
    }

    /* for very large n (outside the cache range) compute iteratively
       without using the cache.  This is harmless because the
       performance requirement is only up to about n = 10 000. */
    seq_t prev2 = 1;           /* a(0) */
    seq_t prev1 = 2;           /* a(1) */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t cur = prev1 / 2 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}

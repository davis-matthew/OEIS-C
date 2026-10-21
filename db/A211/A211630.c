/*
 * A211630  Number of ordered triples (w,x,y) with all terms in {-n, ..., -1,
 *          1, ..., n} and 5w + x + y > 0.
 * https://oeis.org/A211630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-2) + a(n-3) + a(n-5) - 3*a(n-6) +
 *            3*a(n-7) - a(n-8) for n > 7; a(0) = 0, a(1) = 4, a(2) = 32, a(3)
 *            = 106, a(4) = 252, a(5) = 495, a(6) = 855, a(7) = 1359
 * Range:     u8 n <= 4, u16 n <= 25, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoization array */
static idx_t  cache_len = 0;         /* allocated size of cache (elements) */
static idx_t  computed_max = 0;       /* largest n for which a(n) is computed */

/* Ensure cache has space for indices up to n (inclusive).  Initialize
   the first eight terms if this is the first use.  */
static void ensure_cache(idx_t n)
{
    if (!cache) { /* first initialization */
        size_t cap = (size_t)(n > 7 ? n + 1 : 8);
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return; /* allocation failure, caller will handle 0 result */

        /* initial values */
        cache[0] = 0;
        cache[1] = 4;
        cache[2] = 32;
        cache[3] = 106;
        cache[4] = 252;
        cache[5] = 495;
        cache[6] = 855;
        cache[7] = 1359;

        cache_len    = cap;
        computed_max = 7;
    } else if ((idx_t)(n + 1) > cache_len) { /* need more space */
        size_t new_cap = (size_t)(n + 1);
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!cache) return; /* allocation failure */
        cache_len = new_cap;
    }

    /* compute new terms if necessary */
    if (n > computed_max) {
        for (idx_t k = computed_max + 1; k <= n; ++k) {
            cache[k] = 3 * cache[k-1]
                     - 3 * cache[k-2]
                     +     cache[k-3]
                     +     cache[k-5]
                     - 3 * cache[k-6]
                     + 3 * cache[k-7]
                     -     cache[k-8];
        }
        computed_max = n;
    }
}

seq_t a(idx_t n)
{
    if (n < 8) { /* directly return one of the known values */
        /* lazily initialise cache for these small indices */
        ensure_cache(n);
        return cache[n];
    }

    /* compute a(n) using memoization */
    ensure_cache(n);
    return cache[n];
}

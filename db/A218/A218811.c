/*
 * A218811  Hilltop maps: number of 2Xn binary arrays indicating the locations
 *          of corresponding elements not exceeded by any horizontal or
 *          antidiagonal neighbor in a random 0..3 2Xn array.
 * https://oeis.org/A218811
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is defined by the following linear recurrence for n > 20:
 *            a(n) = 3*a(n-1) + 3*a(n-2) + 3*a(n-3) + 3*a(n-4) + 3*a(n-5) +
 *            a(n-6) + 5*a(n-7) + 2*a(n-8) + 2*a(n-9) + 2*a(n-10) + 2*a(n-11)
 *            + a(n-12) + a(n-13) - a(n-14) - a(n-15) - a(n-16) - a(n-17) -
 *            a(n-18) - a(n-19) Initial values (n from 1 to...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* external types supplied by the build */
#define seq_t seq_t
#define idx_t idx_t

/* Memoized computation of the sequence */
seq_t a(idx_t n)
{
    /* Handle the case n < 1 (not required by the problem but safe) */
    if (n < 1) return 0;

    /* Static cache holding known terms, 1‑indexed. */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* highest n computed */
    static idx_t cache_size = 0;  /* allocated size (including index 0) */

    /* Initial values for n = 1 .. 23 */
    static const seq_t init[24] = {
        0ULL, /* unused index 0 */
        1ULL, 15ULL, 61ULL, 249ULL, 993ULL, 3969ULL,
        15871ULL, 63467ULL, 253797ULL, 1014889ULL,
        4058377ULL, 16228793ULL, 64896323ULL, 259509919ULL,
        1037738269ULL, 4149747805ULL, 16594171517ULL,
        66357412857ULL, 265352580967ULL, 1061102131539ULL,
        4243176114789ULL, 16967776245057ULL, 67851397847697ULL
    };

    /* Allocate and initialise the cache on first call */
    if (cache == NULL) {
        cache_size = 24;                      /* indices 0..23 */
        cache_cap  = 23;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init[i];
    }

    /* Grow the cache if needed and compute missing terms */
    if (n > cache_cap) {
        /* Allocate new size */
        idx_t new_size = n + 1;                /* include index 0 */
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) {               /* allocation failed */
            /* In a real program you might handle this error;
               here we simply return 0 for safety. */
            return 0;
        }
        cache = new_cache;
        cache_size = new_size;

        /* Compute terms from cache_cap+1 up to n */
        for (idx_t i = cache_cap + 1; i <= n; ++i) {
            seq_t res = 0;

            /* Positive contributions */
            res += (seq_t)3 * cache[i-1];
            res += (seq_t)3 * cache[i-2];
            res += (seq_t)3 * cache[i-3];
            res += (seq_t)3 * cache[i-4];
            res += (seq_t)3 * cache[i-5];
            res += cache[i-6];
            res += (seq_t)5 * cache[i-7];
            res += (seq_t)2 * cache[i-8];
            res += (seq_t)2 * cache[i-9];
            res += (seq_t)2 * cache[i-10];
            res += (seq_t)2 * cache[i-11];
            res += cache[i-12];
            res += cache[i-13];

            /* Negative contributions */
            res -= cache[i-14];
            res -= cache[i-15];
            res -= cache[i-16];
            res -= cache[i-17];
            res -= cache[i-18];
            res -= cache[i-19];

            cache[i] = res;
        }
        cache_cap = n;
    }

    return cache[n];
}

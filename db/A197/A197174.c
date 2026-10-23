/*
 * A197174  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 2,2,1,0,0
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A197174 (OEIS)
   Empirical recurrence: a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3)
                            - a(n-4) - 4*a(n-5) - 2*a(n-6)
                            + a(n-7) + a(n-8)
   Initial values (offset 1): a(1)=1, a(2)=5, a(3)=10, a(4)=23,
   a(5)=58, a(6)=144, a(7)=363, a(8)=917
   The function memoises computed values so that subsequent calls are O(1). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* outside the offset, arbitrary */

    static seq_t *cache = NULL;    /* memoised sequence values */
    static idx_t  max_cached = 0;  /* highest n already computed */

    /* initialise cache on first call */
    if (!cache) {
        /* allocate space for at least the first 8 terms */
        cache = (seq_t*)malloc((9 + 1) * sizeof(seq_t)); /* 1‑based indices */
        if (!cache) return 0; /* allocation failed: return 0 as fallback */

        /* set the first 8 known values */
        cache[1] = 1;
        cache[2] = 5;
        cache[3] = 10;
        cache[4] = 23;
        cache[5] = 58;
        cache[6] = 144;
        cache[7] = 363;
        cache[8] = 917;

        max_cached = 8;
    }

    /* extend the cache if needed */
    if (n > max_cached) {
        idx_t new_size = n + 1;  /* space for indices 0..n */
        seq_t *tmp = (seq_t*)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;     /* allocation failed: return 0 as fallback */
        cache = tmp;

        /* compute terms from max_cached+1 up to n using the recurrence */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            seq_t pos = 2*cache[i-1] + cache[i-2] + 2*cache[i-3]
                      + cache[i-7] + cache[i-8];
            seq_t neg =       cache[i-4] + 4*cache[i-5] + 2*cache[i-6];
            cache[i] = pos - neg;
        }
        max_cached = n;
    }

    return cache[n];
}

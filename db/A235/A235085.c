/*
 * A235085  Number of (n+1) X (6+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 4,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A235085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6*a(n‑1) + 4*a(n‑2) – 87*a(n‑3) + 90*a(n‑4) + 450*a(n‑5)
 *            – 830*a(n‑6) – 936*a(n‑7) + 2737*a(n‑8) + 408*a(n‑9) –
 *            4078*a(n‑10) + 891*a(n‑11) + 2766*a(n‑12) – 816*a(n‑13) –
 *            904*a(n‑14) + 180*a(n‑15) + 120*a(n‑16) with initial terms
 *            a(1)…a(16) as listed in the OEIS entry
 * Range:     u8 none, u16 n <= 6, u32 n <= 21, u64 n <= 48, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t _init[16] = {
    2554ULL, 4514ULL, 7810ULL, 14702ULL, 27218ULL,
    53590ULL, 103702ULL, 210318ULL, 418838ULL, 866298ULL,
    1758206ULL, 3687018ULL, 7583258ULL, 16071562ULL,
    33396914ULL, 71419678ULL
};

seq_t a(idx_t n)
{
    /* The offset is 1.  For n=0 we return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* For n <= 16 we can answer immediately. */
    if (n <= 16)
        return _init[n-1];

    /* Persistent cache for previously computed terms. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* number of values allocated in cache */

    /* Initialise cache with the first 16 terms if needed. */
    if (cache == NULL) {
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;  /* Allocation failure. */
        for (idx_t i = 0; i < 16; ++i)
            cache[i] = _init[i];
    }

    /* Grow cache if requested index is beyond current size. */
    if (cache_size < n) {
        seq_t *tmp = (seq_t *)realloc(cache, n * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;  /* Allocation failure. */
        cache = tmp;
        /* Compute new terms using the recurrence. */

        for (idx_t i = cache_size; i < n; ++i) {
            seq_t val = 
                6 *   cache[i-1] \
              + 4 *   cache[i-2] \
              - 87 *  cache[i-3] \
              + 90 *  cache[i-4] \
              + 450 * cache[i-5] \
              - 830 * cache[i-6] \
              - 936 * cache[i-7] \
              + 2737 *cache[i-8] \
              + 408 * cache[i-9] \
              - 4078*cache[i-10] \
              + 891 * cache[i-11] \
              + 2766 *cache[i-12] \
              - 816 * cache[i-13] \
              - 904 * cache[i-14] \
              + 180 * cache[i-15] \
              + 120 * cache[i-16];
            cache[i] = val;
        }
        cache_size = n;
    }

    return cache[n-1];  /* a(1) is stored at cache[0] */
}

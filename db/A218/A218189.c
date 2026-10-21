/*
 * A218189  Hilltop maps: number of n X 1 binary arrays indicating the
 *          locations of corresponding elements not exceeded by any horizontal
 *          or vertical neighbor in a random 0..3 n X 1 array.
 * https://oeis.org/A218189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5)+a(n-6)+a(n-7),
 *            a(1)=1,a(2)=3,a(3)=7,a(4)=15,a(5)=29,a(6)=57,a(7)=113
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 0‑based cache; cache[0] is unused */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* number of elements allocated */

    static const seq_t base[8] = {0,1,3,7,15,29,57,113};

    if (n == 0) return 0;   /* offset is 1, but return 0 for n==0 */

    idx_t need = n + 1;  /* we need indices 0..n */

    if (cache == NULL) {
        cache_cap = need > 8 ? need : 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return 0;  /* out of memory, unlikely in tests */
        /* initialise up to index 7 with known values */
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = base[i];
    } else if (cache_cap < need) {
        cache_cap = need;
        cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!cache) return 0;
        /* new entries beyond previous cap are uninitialized */
        for (idx_t i = 1; i <= 7 && i < cache_cap; ++i)
            cache[i] = base[i];
    }

    /* compute missing values up to n */
    for (idx_t i = 1; i <= n; ++i) {
        if (i <= 7) continue;   /* already initialised */
        seq_t sum = 0;
        sum += cache[i-1];
        sum += cache[i-2];
        sum += cache[i-3];
        sum += cache[i-4];
        sum += cache[i-5];
        sum += cache[i-6];
        sum += cache[i-7];
        cache[i] = sum;
    }

    return cache[n];
}

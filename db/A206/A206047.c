/*
 * A206047  Number of (n+1) X 2 0..2 arrays with the number of clockwise edge
 *          increases in every 2 X 2 subblock equal to one.
 * https://oeis.org/A206047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 7*a(n-2) - 2*a(n-3) a(1)=48, a(2)=255,
 *            a(3)=1338 (empirical recurrence for the OEIS sequence A206047)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache   = NULL;
    static idx_t cache_len    = 0;   /* number of allocated elements */
    static idx_t computed_len = 0;   /* largest index for which value is known */

    /* initialise once */
    if (!cache) {
        cache_len = 4;                 /* indices 0..3 used (0 unused) */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;          /* allocation failed */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)48;
        cache[2] = (seq_t)255;
        cache[3] = (seq_t)1338;
        computed_len = 3;
    }

    if (n <= computed_len) {
        return cache[n];
    }

    /* ensure we have space in the allocation */
    if (n + 1 > cache_len) {
        idx_t new_len = cache_len * 2;
        if (new_len < n + 1) new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing values up to n */
    for (idx_t i = computed_len + 1; i <= n; ++i) {
        seq_t val = (seq_t)4 * cache[i - 1]
                  + (seq_t)7 * cache[i - 2]
                  - (seq_t)2 * cache[i - 3];
        cache[i] = val;
    }
    computed_len = n;

    return cache[n];
}

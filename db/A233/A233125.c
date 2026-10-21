/*
 * A233125  Number of nX4 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally or vertically, top left
 *          element zero, and 1 appearing before 2 3 and 4, and 2 appearing
 *          before 3 in row major order (unlabeled 6-colorings with no
 *          clashing color pairs).
 * https://oeis.org/A233125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 160*a(n‑1) – 6144*a(n‑2) + 86016*a(n‑3) – 393216*a(n‑4)
 *            a(1) = 10, a(2) = 896, a(3) = 96256, a(4) = 10674176
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache      = NULL;   /* array indexed 0..k, cache[0] unused       */
static idx_t  cache_size = 0;      /* number of allocated elements             */
static idx_t  cache_filled = 0;    /* largest index for which the value is set */

/* enlarge the cache to hold indices up to need (inclusive) */
static void grow_cache(idx_t need)
{
    if (cache_size > need) return;                      /* already enough */
    size_t new_cap = (size_t)(need + 1);                 /* indices 0..need */
    if (cache == NULL)
        cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    else
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_size = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;              /* defined only for n>=1 */

    /* grow the storage as needed */
    grow_cache(n);

    /* initialise first four terms if they have not been set yet */
    if (cache_filled < 1 && n >= 1) {
        cache[1] = (seq_t)10;
        cache_filled = 1;
    }
    if (cache_filled < 2 && n >= 2) {
        cache[2] = (seq_t)896;
        cache_filled = 2;
    }
    if (cache_filled < 3 && n >= 3) {
        cache[3] = (seq_t)96256;
        cache_filled = 3;
    }
    if (cache_filled < 4 && n >= 4) {
        cache[4] = (seq_t)10674176;
        cache_filled = 4;
    }

    /* compute subsequent terms using the 4‑term recurrence */
    for (idx_t i = cache_filled + 1; i <= n; ++i) {
        cache[i] = (seq_t)160 * cache[i-1]
                 - (seq_t)6144 * cache[i-2]
                 + (seq_t)86016 * cache[i-3]
                 - (seq_t)393216 * cache[i-4];
    }
    cache_filled = n;
    return cache[n];
}

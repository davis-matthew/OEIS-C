/*
 * A151251  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (0, 0,
 *          1), (0, 1, 0), (1, 1, 0), (1, 1, 1)}.
 * https://oeis.org/A151251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of walks of length n in the first octant, starting
 *            at (0,0,0) and using steps (-1,-1,0), (0,0,1), (0,1,0), (1,1,0),
 *            (1,1,1). This is counted by a 1‑dimensional walk on the
 *            x‑coordinate with step values {-1,0,0,+1,+1} (multiplicities
 *            1,2,2,2 respectively), restricted to stay non‑nega...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state for memoisation */
    static seq_t *prev = NULL;        /* counts after current step count */
    static seq_t *curr = NULL;        /* temporary storage for next step */
    static seq_t *cache = NULL;       /* a(k) for k <= max_n */
    static size_t dp_cap = 0;         /* capacity of prev/curr arrays  */
    static size_t cache_cap = 0;      /* capacity of cache array      */
    static idx_t max_n = 0;           /* largest n already processed  */

    /* first call initialisation */
    if (!prev) {
        dp_cap = 1;
        prev = (seq_t *)calloc(dp_cap, sizeof(seq_t));
        curr = (seq_t *)calloc(dp_cap, sizeof(seq_t));
        /* a(0) = 1: the empty walk */
        prev[0] = 1;

        cache_cap = 1;
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        cache[0] = 1;
    }

    /* enlarge cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* zero the new part (not strictly needed) */
        for (size_t i = cache_cap; i < new_cap; ++i) cache[i] = 0;
        cache_cap = new_cap;
    }

    /* already computed? */
    if (n <= max_n)
        return cache[n];

    /* compute successive steps up to n */
    for (idx_t step = max_n + 1; step <= n; ++step) {
        /* ensure enough capacity for height up to step */
        size_t need_cap = (size_t)step + 1;
        if (dp_cap < need_cap) {
            dp_cap = need_cap;
            prev = (seq_t *)realloc(prev, dp_cap * sizeof(seq_t));
            curr = (seq_t *)realloc(curr, dp_cap * sizeof(seq_t));
        }

        /* zero destination array */
        for (size_t i = 0; i <= step; ++i)
            curr[i] = 0;

        /* transition */
        for (size_t h = 0; h < (size_t)step; ++h) {
            seq_t val = prev[h];
            /* stay (B or C) */
            curr[h] += val * 2;
            /* up (D or E) */
            curr[h + 1] += val * 2;
            /* down (A), only if h > 0 to avoid negative height */
            if (h > 0)
                curr[h - 1] += val;
        }

        /* sum over all final heights to obtain a(step) */
        seq_t sum = 0;
        for (size_t h = 0; h <= step; ++h)
            sum += curr[h];

        cache[step] = sum;

        /* prepare for next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
        max_n = step;
    }

    return cache[n];
}

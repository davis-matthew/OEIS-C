/*
 * A151019  Number of walks within N^3 (the first octant of Z^3) starting at
 *          (0,0,0) and consisting of n steps taken from {(-1, -1, 0), (-1,
 *          -1, 1), (-1, 0, 0), (1, 1, 0), (1, 1, 1)}.
 * https://oeis.org/A151019
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = number of walks in N^3 with steps (-1,-1,0), (-1,-1,1),
 *            (-1,0,0), (1,1,0), (1,1,1), starting at (0,0,0) and staying
 *            within the first octant. The computation is performed by dynamic
 *            programming on the (x‑y) and x coordinates, reducing the problem
 *            to a 1D walk with step weights 2 (up) and 3...
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, free, realloc */

seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;

    /* dynamic arrays for current and next height distributions */
    static seq_t *cur = NULL;
    static idx_t cur_len = 0;
    static seq_t *next = NULL;
    static idx_t next_len = 0;

    /* initialise once */
    if (cache_len == 0) {
        /* cache for a(0) */
        cache_cap = 16;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
        cache[0] = (seq_t)1;
        cache_len = 1;

        /* current height distribution for n=0 (only height 0) */
        cur_len = 1;
        cur = (seq_t*)malloc(sizeof(seq_t) * cur_len);
        cur[0] = (seq_t)1;

        /* next array (will be resized as needed) */
        next_len = 2;
        next = (seq_t*)malloc(sizeof(seq_t) * next_len);
    }

    /* extend precomputation up to requested n */
    if (n >= cache_len) {
        for (idx_t step = cache_len - 1; step < n; ++step) {
            size_t needed = (size_t)cur_len + 1;          /* new heights after this step */
            if (next_len < needed) {                     /* grow next buffer */
                next_len = needed;
                next = (seq_t*)realloc(next, sizeof(seq_t) * next_len);
            }
            /* clear next buffer */
            for (size_t i = 0; i < needed; ++i)
                next[i] = (seq_t)0;

            seq_t sum_next = (seq_t)0;

            for (idx_t h = 0; h < cur_len; ++h) {
                seq_t val = cur[h];
                if (val == (seq_t)0) continue;

                /* up step: (x,y) -> (x+1,y+1)   weight 2 */
                seq_t up_val = val * (seq_t)2;
                next[h + 1] += up_val;
                sum_next += up_val;

                /* down step: (-1,-1)   weight 3 (steps 1 or 2) */
                if (h > 0) {
                    seq_t down_val = val * (seq_t)3;
                    next[h - 1] += down_val;
                    sum_next += down_val;
                }
            }

            /* swap cur and next for next iteration */
            seq_t *tmp = cur;
            cur   = next;
            next  = tmp;
            cur_len = (idx_t)needed;

            /* store a(step+1) */
            if (cache_len >= cache_cap) {
                cache_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = sum_next;
        }
    }

    return cache[n];
}

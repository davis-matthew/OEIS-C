/*
 * A232377  Number of 2Xn 0..3 arrays with every 0 next to a 1, every 1 next
 *          to a 2 and every 2 next to a 3 horizontally, diagonally or
 *          antidiagonally, and no adjacent values equal.
 * https://oeis.org/A232377
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 31, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A232377: Number of 2×n 0..3 arrays with
   (i) each 0 adjacent to a 1,
       each 1 adjacent to a 2,
       each 2 adjacent to a 3,
       adjacency means horizontally, diagonally or antidiagonally
   (ii) no adjacent equal values in those directions.
   The DP proceeds from left to right, keeping for each column
   (top value, bottom value, unsatisfied flag for top, unsatisfied flag for bottom).
   The flag is 1 if the cell still needs a neighbor of value+1 on the right.
   Vertical adjacency is NOT considered for either constraints or for equalities. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* 64 possible states: (top, bottom, unsat_top, unsat_bottom) */
    static seq_t dp_cur[64], dp_next[64];
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_up_to = 0;
    static int initialized = 0;

    /* Initialise for the first column (n = 1). */
    if (!initialized) {
        for (idx_t i = 0; i < 64; ++i) dp_cur[i] = 0;
        /* No left neighbours, so every cell with value < 3 remains unsatisfied. */
        for (int a0 = 0; a0 <= 3; ++a0) {
            for (int a1 = 0; a1 <= 3; ++a1) {
                /* vertical adjacency is not forbidden */
                int ua0 = (a0 == 3) ? 0 : 1;     /* no left neighbour */
                int ua1 = (a1 == 3) ? 0 : 1;
                idx_t idx = ((idx_t)a0 << 4) | ((idx_t)a1 << 2)
                            | ((idx_t)ua0 << 1) | (idx_t)ua1;
                dp_cur[idx] += 1;
            }
        }
        /* Allocate cache for n = 1. */
        if (cache == NULL || cache_cap < 2) {
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * 2);
            cache = tmp;
            cache_cap = 2;
        }
        seq_t sum = 0;
        for (idx_t idx = 0; idx < 64; ++idx) {
            if (((idx >> 1) & 1) == 0 && (idx & 1) == 0) {
                sum += dp_cur[idx];
            }
        }
        cache[1] = sum;      /* a(1) = 1 */
        cached_up_to = 1;
        initialized = 1;
    }

    /* Grow cache if needed. */
    if (n > cached_up_to) {
        if (cache == NULL || cache_cap <= (size_t)n) {
            size_t new_cap = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
            cache = tmp;
            cache_cap = new_cap;
        }

        for (idx_t cols = cached_up_to + 1; cols <= n; ++cols) {
            /* Clear next DP array. */
            for (idx_t i = 0; i < 64; ++i) dp_next[i] = 0;

            for (idx_t cur = 0; cur < 64; ++cur) {
                seq_t ways = dp_cur[cur];
                if (ways == 0) continue;
                int x0 = (cur >> 4) & 3;    /* previous top */
                int x1 = (cur >> 2) & 3;    /* previous bottom */
                int ux0 = (cur >> 1) & 1;   /* unsatisfied top */
                int ux1 = cur & 1;          /* unsatisfied bottom */

                for (int a0 = 0; a0 <= 3; ++a0) {
                    for (int a1 = 0; a1 <= 3; ++a1) {
                        /* no equal adjacency in allowed directions */
                        if (a0 == x0 || a1 == x1 || a0 == x1 || a1 == x0)
                            continue;
                        /* satisfy unsatisfied previous cells */
                        if (ux0) {
                            if (!(a0 == x0 + 1 || a1 == x0 + 1))
                                continue;
                        }
                        if (ux1) {
                            if (!(a1 == x1 + 1 || a0 == x1 + 1))
                                continue;
                        }
                        /* compute unsatisfied flags for current cells */
                        int ua0 = (a0 == 3) ? 0 :
                                   ((x0 == a0 + 1 || x1 == a0 + 1) ? 0 : 1);
                        int ua1 = (a1 == 3) ? 0 :
                                   ((x1 == a1 + 1 || x0 == a1 + 1) ? 0 : 1);
                        idx_t nxt = ((idx_t)a0 << 4) | ((idx_t)a1 << 2)
                                    | ((idx_t)ua0 << 1) | (idx_t)ua1;
                        dp_next[nxt] += ways;
                    }
                }
            }
            /* swap DP arrays */
            for (idx_t i = 0; i < 64; ++i) dp_cur[i] = dp_next[i];

            /* count complete arrays after this column */
            seq_t s = 0;
            for (idx_t j = 0; j < 64; ++j) {
                if (((j >> 1) & 1) == 0 && (j & 1) == 0) {
                    s += dp_cur[j];
                }
            }
            cache[cols] = s;
        }
        cached_up_to = n;
    }

    return cache[n];
}

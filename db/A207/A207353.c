/*
 * A207353  Number of nX3 0..2 arrays avoiding the pattern z+1 z+1 z
 *          horizontally and z-1 z-1 z vertically.
 * https://oeis.org/A207353
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   build_rows_and_transitions
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = number of n×3 arrays over {0,1,2} avoiding
 * horizontal pattern z+1 z+1 z and vertical pattern z-1 z-1 z.
 * Computed by dynamic programming over the last two rows.
 * a(1)=25, a(2)=625.
 */
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/build_rows_and_transitions.h"

/* seq_t and idx_t are defined by the build system.  */
/* This file defines only the requested function.  */

seq_t a(idx_t n)
{
    /* 25 possible rows that already avoid the horizontal pattern  */
    static int rows[25][3];
    static unsigned char row_valid[27] = {0};
    static bool rows_inited = false;

    static seq_t* dp = NULL;          /* counts for sequences of current length */
    static seq_t* dp_next = NULL;
    static const size_t STATE_CNT = 25 * 25;   /* 625 */

    static seq_t* cache = NULL;      /* memoised a(k) values */
    static idx_t cache_len = 0;       /* number of entries in cache (index 0 unused) */

    static idx_t max_comp = 0;       /* maximum n for which a(n) is known */

    if (!rows_inited) {
        /* build rows, excluding the two forbidden horizontal patterns */
        unsigned char id[27];
        memset(id, 255, sizeof(id));
        int rid = 0;
        for (int enc = 0; enc < 27; ++enc) {
            /* encode base-3 digits: d0 + 3*d1 + 9*d2 */
            int d0 = enc % 3;
            int d1 = (enc / 3) % 3;
            int d2 = (enc / 9) % 3;
            /* forbid patterns 1,1,0 (enc 4) and 2,2,1 (enc 17) */
            if (enc == 4 || enc == 17)
                continue;
            rows[rid][0] = d0;
            rows[rid][1] = d1;
            rows[rid][2] = d2;
            id[enc] = (unsigned char)rid;
            ++rid;
        }
        /* allocate dp arrays */
        dp = (seq_t*)malloc(STATE_CNT * sizeof(seq_t));
        dp_next = (seq_t*)malloc(STATE_CNT * sizeof(seq_t));
        memset(dp, 0, STATE_CNT * sizeof(seq_t));
        memset(dp_next, 0, STATE_CNT * sizeof(seq_t));

        /* cache for a(1) and a(2) */
        cache_len = 2 + 1;   /* index 0 unused */
        cache = (seq_t*)calloc(cache_len, sizeof(seq_t));
        cache[1] = (seq_t)25;
        cache[2] = (seq_t)625;
        max_comp = 2;

        rows_inited = true;
    }

    if (n < 1)
        return (seq_t)0;      /* undefined, but safe */

    /* return from cache if already computed */
    if (n <= max_comp)
        return cache[n];

    /* extend cache up to n */
    for (idx_t cur = max_comp + 1; cur <= n; ++cur) {
        /* For cur == 3, initialise dp for length 2 (all pairs allowed) */
        if (cur == 3) {
            for (size_t s = 0; s < STATE_CNT; ++s)
                dp[s] = (seq_t)1;
        }

        /* reset dp_next for new length cur */
        memset(dp_next, 0, STATE_CNT * sizeof(seq_t));

        for (size_t s = 0; s < STATE_CNT; ++s) {
            seq_t val = dp[s];
            if (!val)
                continue;
            unsigned prev = s / 25;   /* previous row index */
            unsigned cur_row = s % 25;/* current row index */

            for (unsigned r = 0; r < 25; ++r) {
                /* Check vertical pattern with rows prev, cur_row, r */
                bool ok = true;
                for (int col = 0; col < 3; ++col) {
                    int a = rows[prev][col];
                    int b = rows[cur_row][col];
                    int c = rows[r][col];
                    if (a == b && c == a + 1) {
                        ok = false;
                        break;
                    }
                }
                if (!ok)
                    continue;
                size_t ns = cur_row * 25 + r;   /* new state: (cur_row, r) */
                dp_next[ns] += val;
            }
        }

        /* compute total for this length */
        seq_t total = (seq_t)0;
        for (size_t s = 0; s < STATE_CNT; ++s)
            total += dp_next[s];

        /* cache the result */
        if (cur >= cache_len) {
            size_t new_len = (size_t)cur + 1;
            cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
            memset(cache + cache_len, 0, (new_len - cache_len) * sizeof(seq_t));
            cache_len = new_len;
        }
        cache[cur] = total;
        max_comp = cur;

        /* swap dp and dp_next for next iteration */
        seq_t* tmp = dp;
        dp = dp_next;
        dp_next = tmp;
    }

    return cache[n];
}

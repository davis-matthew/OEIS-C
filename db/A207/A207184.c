/*
 * A207184  Number of 5Xn 0..1 arrays avoiding 0 0 0 horizontally and 0 1 0
 *          vertically.
 * https://oeis.org/A207184
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 5×n binary arrays avoiding 000 horizontally and
 *            010 vertically. Computed by dynamic programming over columns
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 32,
 *            big any n
 * Helpers:   is_valid_column
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint8_t */
#include "../utils/is_valid_column.h"

/*------------------------------------------------------------*/

/*------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* pre‑compute list of allowed 5‑bit columns (0…31)   */
    uint8_t valid_cols[32];
    int valid_cnt = 0;
    for (uint8_t col = 0; col < 32; ++col)
        if (is_valid_column(col))
            valid_cols[valid_cnt++] = col;

    /* state encoding: 10 bits = (c1<<5) | c2   */
    const int STATE_SIZE = 1 << 10;         /* 1024 */
    seq_t dp_cur[STATE_SIZE];
    seq_t dp_next[STATE_SIZE];

    /* initialize DP with a dummy column of all ones (31) for the two
       missing previous columns; this guarantees no horizontal 000
       can arise in early columns.                            */
    for (int i = 0; i < STATE_SIZE; ++i) {
        dp_cur[i]  = 0;
        dp_next[i] = 0;
    }
    const uint8_t DUMMY = 31;                 /* 11111b */
    int init_state = (int)((DUMMY << 5) | DUMMY);
    dp_cur[init_state] = 1;

    /* dynamic programming over columns                           */
    for (idx_t col = 0; col < n; ++col) {
        /* reset next array */
        for (int i = 0; i < STATE_SIZE; ++i)
            dp_next[i] = 0;

        for (int state = 0; state < STATE_SIZE; ++state) {
            seq_t ways = dp_cur[state];
            if (ways == 0) continue;

            uint8_t c1 = (uint8_t)(state >> 5);   /* column k-2   */
            uint8_t c2 = (uint8_t)(state & 31);   /* column k-1   */

            /* try all possible next columns (column k)          */
            for (int vi = 0; vi < valid_cnt; ++vi) {
                uint8_t c3 = valid_cols[vi];

                /* horizontal 000 check for each of 5 rows     */
                int bad = 0;
                for (int r = 0; r < 5; ++r) {
                    int b1 = (c1 >> r) & 1;
                    int b2 = (c2 >> r) & 1;
                    int b3 = (c3 >> r) & 1;
                    if (!b1 && !b2 && !b3) { bad = 1; break; }
                }
                if (bad) continue;

                int new_state = (int)((c2 << 5) | c3);
                dp_next[new_state] += ways;
            }
        }
        /* swap dp_cur and dp_next for next iteration */
        for (int i = 0; i < STATE_SIZE; ++i)
            dp_cur[i] = dp_next[i];
    }

    /* sum all states to obtain total number of arrays              */
    seq_t result = 0;
    for (int i = 0; i < STATE_SIZE; ++i)
        result += dp_cur[i];

    return result;
}

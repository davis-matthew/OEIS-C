/*
 * A203043  Number of (n+3)X6 binary arrays with no more than one of any
 *          consecutive four bits set in any row or column.
 * https://oeis.org/A203043
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 9, u64 n <= 23, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A203043: number of (n+3)×6 binary arrays with at most one 1 in any
    consecutive block of 4 cells in a row or a column.
    We count sequences of rows; each row is one of the 10 patterns
    with no two 1's closer than 4 columns.
    When adding a new row it may not contain a 1 in any column
    that already has a 1 in any of the previous three rows.
    The state after i rows is the last three rows,
    each represented by a pattern index (0..9).
    Transition: (a,b,c) ---> (r,a,b) if
        (pat[r] & pat[a])==0 &&
        (pat[r] & pat[b])==0 &&
        (pat[r] & pat[c])==0
    Total after m=n+3 rows is the sum of all ending states.  */

#include <stdbool.h>
#include <stddef.h>

static const int pat[10] = {
    0,          /* 0 */
    1,          /* 000001 */
    2,          /* 000010 */
    4,          /* 000100 */
    8,          /* 001000 */
    16,         /* 010000 */
    32,         /* 100000 */
    17,         /* 010001 (0 & 4) */
    33,         /* 100001 (0 & 5) */
    34          /* 100010 (1 & 5) */
};

seq_t a(idx_t n)
{
    /* number of rows */
    idx_t m = n + 3U;
    /* dp arrays: 1000 = 10^3 states (a,b,c indices 0..9) */
    seq_t dp_cur[1000];
    seq_t dp_next[1000];

    /* initialize dp_cur to 0 */
    for (int i = 0; i < 1000; ++i) dp_cur[i] = 0;
    dp_cur[0] = 1;   /* starting state (0,0,0) */

    /* iterate rows */
    for (size_t row = 0; row < (size_t)m; ++row) {
        /* zero dp_next */
        for (int i = 0; i < 1000; ++i) dp_next[i] = 0;

        for (int s = 0; s < 1000; ++s) {
            seq_t cur_val = dp_cur[s];
            if (cur_val == 0) continue;

            int a0 = s / 100;          /* last row */
            int a1 = (s / 10) % 10;    /* previous row */
            int a2 = s % 10;           /* two rows before */

            int bits0 = pat[a0];
            int bits1 = pat[a1];
            int bits2 = pat[a2];

            for (int r = 0; r < 10; ++r) {
                int br = pat[r];
                if ((br & bits0) || (br & bits1) || (br & bits2))
                    continue;   /* column constraint violated */

                int next = r * 100 + a0 * 10 + a1;  /* new state */
                dp_next[next] = dp_next[next] + cur_val;
            }
        }

        /* swap dp_cur and dp_next by copying */
        for (int i = 0; i < 1000; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all states */
    seq_t result = 0;
    for (int i = 0; i < 1000; ++i)
        result = result + dp_cur[i];

    return result;
}

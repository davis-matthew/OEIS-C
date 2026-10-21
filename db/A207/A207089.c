/*
 * A207089  Number of 3 X n 0..1 arrays avoiding 0 0 0 horizontally and 0 0 1
 *          vertically.
 * https://oeis.org/A207089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* DP recurrence: a(1)=7, a(2)=49.
   For n ≥ 3 the number of 3×n 0–1 arrays that avoid a horizontal
   triple 000 and a vertical triple 001 is obtained by
   maintaining the last two columns.  Each column is one of the 8
   3‑bit patterns; the pattern 001 is forbidden.  A new column
   may be appended iff, for every row, the three consecutive bits
   in that row are not all 0.  The function sums all valid
   state counts after processing n columns. */

#include <stddef.h>   /* for size_t */

static const int allowed[7] = {0, 2, 3, 4, 5, 6, 7};

seq_t a(idx_t n)
{
    /* n < 1 is outside the offset; return 0 harmlessly */
    if (n < (idx_t)1) return (seq_t)0;

    /* base cases */
    if (n == (idx_t)1) return (seq_t)7;
    if (n == (idx_t)2) return (seq_t)49;

    /* dp_cur[state] stores the number of arrays of the current length
       ending with the two most recent columns represented by 'state',
       where state = (prev_prev << 3) | prev  (values 0..63).   */
    seq_t dp_cur[64] = {0};
    seq_t dp_next[64] = {0};

    /* initialise for length 2: any allowed pair of columns is valid */
    const size_t allowed_len = sizeof(allowed) / sizeof(allowed[0]);
    for (size_t i = 0; i < allowed_len; ++i) {
        int a_col = allowed[i];
        for (size_t j = 0; j < allowed_len; ++j) {
            int b_col = allowed[j];
            size_t state = ((size_t)a_col << 3) | (size_t)b_col;
            dp_cur[state] = (seq_t)1;
        }
    }

    /* extend the array from length 3 up to n */
    for (idx_t step = 3; step <= n; ++step) {
        /* clear next DP table */
        for (size_t i = 0; i < 64; ++i) dp_next[i] = (seq_t)0;

        for (size_t s = 0; s < 64; ++s) {
            seq_t ways = dp_cur[s];
            if (ways == (seq_t)0) continue;

            int a_col = (int)(s >> 3);
            int b_col = (int)(s & 7);

            /* try each possible new column */
            for (size_t k = 0; k < allowed_len; ++k) {
                int c_col = allowed[k];

                /* horizontal 000 check for each row */
                int a0 = a_col & 1, b0 = b_col & 1, c0 = c_col & 1;
                if (a0 == 0 && b0 == 0 && c0 == 0) continue;

                int a1 = (a_col >> 1) & 1, b1 = (b_col >> 1) & 1, c1 = (c_col >> 1) & 1;
                if (a1 == 0 && b1 == 0 && c1 == 0) continue;

                int a2 = (a_col >> 2) & 1, b2 = (b_col >> 2) & 1, c2 = (c_col >> 2) & 1;
                if (a2 == 0 && b2 == 0 && c2 == 0) continue;

                size_t new_state = ((size_t)b_col << 3) | (size_t)c_col;
                dp_next[new_state] += ways;
            }
        }

        /* copy dp_next to dp_cur for the next iteration */
        for (size_t i = 0; i < 64; ++i) dp_cur[i] = dp_next[i];
    }

    /* sum all states to obtain the final count */
    seq_t result = (seq_t)0;
    for (size_t i = 0; i < 64; ++i) result += dp_cur[i];
    return result;
}

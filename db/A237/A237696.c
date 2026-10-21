/*
 * A237696  Number of (n+1)X(1+1) 0..2 arrays with the maximum plus the upper
 *          median plus the lower median of every 2X2 subblock differing from
 *          its horizontal and vertical neighbors by exactly one.
 * https://oeis.org/A237696
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: 
   a(1) = 81. For n >= 2, a(n) is obtained by dynamic programming
   over row states of a (n+1)×2 array over {0,1,2} such that for each
   consecutive pair of rows R_i, R_{i+1} let
       S_i = sum of the three largest entries in the 2×2 block of
             (R_i,R_{i+1}).  Then |S_i - S_{i+1}| must equal 1.
   The DP counts all sequences of rows satisfying this condition. */
seq_t a(idx_t n)
{
    /* precompute the value S for every pair of rows (9×9 table) */
    int S[9][9];
    for (int r1 = 0; r1 < 9; ++r1) {
        int a = r1 / 3, b = r1 % 3;
        for (int r2 = 0; r2 < 9; ++r2) {
            int c = r2 / 3, d = r2 % 3;
            int sum = a + b + c + d;
            int mn = a;
            if (b < mn) mn = b;
            if (c < mn) mn = c;
            if (d < mn) mn = d;
            S[r1][r2] = sum - mn;          /* sum of the top three values */
        }
    }

    /* DP arrays: dp_cur[row][S_value] = number of ways to reach this state */
    seq_t dp_cur[9][7] = {0};
    seq_t dp_next[9][7] = {0};

    /* initialize after first two rows (i.e., first subblock) */
    for (int r1 = 0; r1 < 9; ++r1) {
        for (int r2 = 0; r2 < 9; ++r2) {
            int s = S[r1][r2];
            dp_cur[r2][s] += 1;
        }
    }

    /* iterate over remaining rows */
    idx_t step = 1;              /* number of subblocks processed so far */
    while (step < n) {
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 7; ++j)
                dp_next[i][j] = 0;

        for (int r_prev = 0; r_prev < 9; ++r_prev) {
            for (int s_prev = 0; s_prev < 7; ++s_prev) {
                seq_t val = dp_cur[r_prev][s_prev];
                if (!val) continue;
                for (int r_next = 0; r_next < 9; ++r_next) {
                    int s_cur = S[r_prev][r_next];
                    if (s_cur == s_prev + 1 || s_cur == s_prev - 1) {
                        dp_next[r_next][s_cur] += val;
                    }
                }
            }
        }
        /* copy dp_next into dp_cur for next iteration */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 7; ++j)
                dp_cur[i][j] = dp_next[i][j];

        ++step;
    }

    /* sum all counts to get total number of arrays */
    seq_t result = 0;
    for (int r = 0; r < 9; ++r)
        for (int s = 0; s < 7; ++s)
            result += dp_cur[r][s];

    return result;
}

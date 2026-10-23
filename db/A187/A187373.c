/*
 * A187373  Half the number of (n+2)X3 0..3 arrays with each 3X3 subblock
 *          having sum 13 or 14.
 * https://oeis.org/A187373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ½·(number of (n+2)×3 arrays with 0..3 entries such that
 *            every 3×3 subblock sums to 13 or 14). The computation uses a
 *            dynamic‑programming algorithm on row sums (0–9)
 * Range:     u8 none, u16 n <= 1, u32 n <= 4, u64 n <= 12, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Count of 3‑digit base‑4 strings having sum s (s = 0..9). */
static const seq_t h_row[10] = {1, 3, 6, 10, 12, 12, 10,  6,  3,  1};

/* For each pair (a,b) of row sums, list the admissible next sums s
   such that a+b+s = 13 or 14.  At most two values per pair. */
static int opt_cnt[10][10];
static int opt_s[10][10][2];
static bool init_done = false;

/* Helper to initialise opt arrays – executed once. */
static void init_opt(void) {
    if (init_done) return;
    for (int a = 0; a < 10; ++a) {
        for (int b = 0; b < 10; ++b) {
            int c = a + b;
            int k = 0;
            int s1 = 13 - c;
            if (0 <= s1 && s1 <= 9) {
                opt_s[a][b][k++] = s1;
            }
            int s2 = 14 - c;
            if (0 <= s2 && s2 <= 9 && s2 != s1) {
                opt_s[a][b][k++] = s2;
            }
            opt_cnt[a][b] = k;
        }
    }
    init_done = true;
}

seq_t a(idx_t n) {
    if (n == (idx_t)0) return 0;

    init_opt();

    /* Number of rows in the array: (n+2). */
    idx_t rows = n + 2;

    /* DP arrays: dp[a][b] counts sequences ending with last two sums a,b. */
    seq_t dp[10][10] = {0};
    seq_t ndp[10][10] = {0};

    /* Initialise for the first two rows. */
    for (int a = 0; a < 10; ++a) {
        for (int b = 0; b < 10; ++b) {
            dp[a][b] = h_row[a] * h_row[b];
        }
    }

    /* Process each subsequent row, from 3 up to "rows".  Each step adds
       one new row while ensuring the 3×3 sum condition. */
    for (idx_t step = 3; step <= rows; ++step) {
        /* Zero ndp. */
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                ndp[i][j] = 0;

        /* Transition. */
        for (int a = 0; a < 10; ++a) {
            for (int b = 0; b < 10; ++b) {
                seq_t cur = dp[a][b];
                if (cur == 0) continue;
                int k = opt_cnt[a][b];
                for (int t = 0; t < k; ++t) {
                    int s = opt_s[a][b][t];
                    ndp[b][s] += cur * h_row[s];
                }
            }
        }

        /* Swap dp and ndp for next iteration. */
        for (int i = 0; i < 10; ++i)
            for (int j = 0; j < 10; ++j)
                dp[i][j] = ndp[i][j];
    }

    /* Sum all counts for complete arrays. */
    seq_t total = 0;
    for (int a = 0; a < 10; ++a)
        for (int b = 0; b < 10; ++b)
            total += dp[a][b];

    /* The sequence is half the number of such arrays. */
    return total / 2;
}

/*
 * A207392  Number of 3Xn 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207392
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of 3×n binary arrays avoiding horizontally the
 *            patterns 001 and 010, and vertically the patterns 001 and 110
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 20, u64 n <= 43, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Allowed column patterns (3 bits).  */
    static const uint8_t pats[6] = {0, 2, 3, 4, 5, 7};   /* 000,010,011,100,101,111 */

    /* Pre‑compute transition table: valid[a][b][c] means
       the triple of columns (a,b,c) satisfies the horizontal
       constraints for all three rows.  */
    static bool valid[6][6][6];
    static bool ready = false;
    if (!ready) {
        for (int a = 0; a < 6; ++a) {
            for (int b = 0; b < 6; ++b) {
                for (int c = 0; c < 6; ++c) {
                    bool ok = true;
                    for (int r = 0; r < 3 && ok; ++r) {
                        int d0 = (pats[a] >> (2 - r)) & 1;
                        int d1 = (pats[b] >> (2 - r)) & 1;
                        int d2 = (pats[c] >> (2 - r)) & 1;
                        /* Forbidden horizontal patterns: 001 or 010. */
                        if ((d0 == 0 && d1 == 0 && d2 == 1) ||
                            (d0 == 0 && d1 == 1 && d2 == 0))
                            ok = false;
                    }
                    valid[a][b][c] = ok;
                }
            }
        }
        ready = true;
    }

    /* Special cases n = 0,1,2.  */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)6;          /* any single allowed column */
    if (n == 2) return (seq_t)36;         /* any pair of allowed columns */

    /* Dynamic programming: dp[p][q] = number of arrays ending with
       columns (p, q) after having processed the current number of columns. */
    seq_t dp[6][6]   = {0};
    seq_t next[6][6] = {0};

    /* Initialise for length 2.  */
    for (int p = 0; p < 6; ++p)
        for (int q = 0; q < 6; ++q)
            dp[p][q] = (seq_t)1;

    /* Extend to length 3,4,...,n.  */
    for (idx_t len = 3; len <= n; ++len) {
        /* Zero the next array.  */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                next[i][j] = (seq_t)0;

        /* Transition: for each previous pair (a,b) and each candidate c.  */
        for (int a = 0; a < 6; ++a) {
            for (int b = 0; b < 6; ++b) {
                seq_t val = dp[a][b];
                if (!val) continue;
                for (int c = 0; c < 6; ++c) {
                    if (valid[a][b][c])
                        next[b][c] += val;
                }
            }
        }
        /* Swap dp and next.  */
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                dp[i][j] = next[i][j];
    }

    /* Sum all end states.  */
    seq_t result = (seq_t)0;
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            result += dp[i][j];

    return result;
}

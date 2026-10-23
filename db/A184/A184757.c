/*
 * A184757  Half the number of n X 5 binary arrays with no 1 having an
 *          adjacent 1 both above and to its left.
 * https://oeis.org/A184757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A184757:  a(n) = number of n×5 binary arrays with no “1” having both an adjacent
   1 above and to its left.  The sequence satisfies the recurrence
   a(n)=13*a(n‑1)+150*a(n‑2)+550*a(n‑3)+889*a(n‑4)+434*a(n‑5)
       –228*a(n‑6)+32*a(n‑7)   for n≥8,
   with initial values
   a(1)=16, a(2)=317, a(3)=6847, a(4)=145778,
   a(5)=3110914, a(6)=66363023, a(7)=1415755252.  The implementation
   below counts the arrays by dynamic programming on row patterns. */

#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Transition table: for each previous row pattern p (0..31)
       list all next row patterns c (0..31) that satisfy the
       “no 1 with both up and left 1” property. */
    static bool init = false;
    static uint8_t allow[32][32];
    static uint8_t allowCnt[32];

    if (!init) {
        init = true;
        for (int p = 0; p < 32; ++p) {
            allowCnt[p] = 0;
            for (int c = 0; c < 32; ++c) {
                bool ok = true;
                for (int j = 1; j <= 5 && ok; ++j) {
                    if (c & (1 << (j - 1))) {          /* cell (j) is 1 */
                        if (j >= 2) {                    /* left neighbour exists */
                            if ((p & (1 << (j - 1))) && (c & (1 << (j - 2)))) {
                                /* both up and left are 1 – forbidden */
                                ok = false;
                            }
                        }
                    }
                }
                if (ok) {
                    allow[p][allowCnt[p]++] = (uint8_t)c;
                }
            }
        }
    }

    /* Dynamic programming over rows */
    seq_t dp[32], next[32];
    for (int i = 0; i < 32; ++i) dp[i] = 1;     /* first row: all patterns allowed */

    for (idx_t row = 2; row <= n; ++row) {
        for (int i = 0; i < 32; ++i) next[i] = 0;
        for (int p = 0; p < 32; ++p) {
            seq_t val = dp[p];
            if (val == 0) continue;
            uint8_t *list = allow[p];
            uint8_t cnt   = allowCnt[p];
            for (uint8_t k = 0; k < cnt; ++k) {
                int c = list[k];
                next[c] += val;
            }
        }
        for (int i = 0; i < 32; ++i) dp[i] = next[i];
    }

    /* Total number of arrays */
    seq_t total = 0;
    for (int i = 0; i < 32; ++i) total += dp[i];

    /* The problem statement counts only half of the arrangements */
    return total / 2;
}

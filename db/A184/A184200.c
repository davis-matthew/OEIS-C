/*
 * A184200  Number of (n+1) X 3 binary arrays with no 2 X 2 subblock
 *          containing fewer than two 1s.
 * https://oeis.org/A184200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of (n+1)×3 binary arrays with no 2×2 subblock
 *            containing fewer than two 1s. (OEIS A184200)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Helper: test whether rows i and j can be consecutive.
   Rows are encoded as 3-bit numbers: bit0 = column0, bit1 = column1, bit2 = column2. */
static bool allowed(uint8_t i, uint8_t j)
{
    /* 2×2 block on columns 0 and 1 */
    unsigned k = ((i >> 0) & 1) + ((i >> 1) & 1) + ((j >> 0) & 1) + ((j >> 1) & 1);
    if (k < 2) return false;
    /* 2×2 block on columns 1 and 2 */
    k = ((i >> 1) & 1) + ((i >> 2) & 1) + ((j >> 1) & 1) + ((j >> 2) & 1);
    return k >= 2;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* n ≥ 1 by definition; for robustness handle n==0. */
    if (n == 0) return (seq_t)8;   /* there are 8 possible single rows */

    const size_t STATES = 8;
    seq_t dp[2][STATES];
    size_t cur = 0, nxt = 1;
    /* initialize dp for 0 transitions (i.e., first row can be anything) */
    for (size_t s = 0; s < STATES; ++s) dp[cur][s] = (seq_t)1;

    /* iterate n transitions (n rows after the first) */
    for (idx_t step = 0; step < n; ++step) {
        /* zero next dp */
        for (size_t s = 0; s < STATES; ++s) dp[nxt][s] = (seq_t)0;
        /* transition */
        for (uint8_t i = 0; i < STATES; ++i) {
            seq_t val = dp[cur][i];
            if (val == 0) continue;
            for (uint8_t j = 0; j < STATES; ++j) {
                if (allowed(i, j))
                    dp[nxt][j] += val;
            }
        }
        /* swap */
        size_t t = cur; cur = nxt; nxt = t;
    }

    /* sum over all final states */
    seq_t res = (seq_t)0;
    for (size_t s = 0; s < STATES; ++s)
        res += dp[cur][s];
    return res;
}

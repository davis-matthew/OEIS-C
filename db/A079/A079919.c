/*
 * A079919  Solution to the Dancing School Problem with 14 girls and n+14
 *          boys: f(14,n).
 * https://oeis.org/A079919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^14 - 77*n^13 + 3094*n^12 - 83083*n^11 + 1637636*n^10 -
 *            24785761*n^9 + 294696402*n^8 - 2779448529*n^7 + 20797459683*n^6
 *            - 122389753486*n^5 + 555826054784*n^4 - 1883902028008*n^3 +
 *            4494445040176*n^2 - 6742111050752*n + 4789534153984 (n ≥ 12) For
 *            n < 12 computed by permanent of the matrix B...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>


seq_t a(idx_t n)
{
    const idx_t g = 14;

    /* Computation for small n by permanent (dynamic programming). */
    if (n < (idx_t)12) {
        enum { MASKS = 1U << 14 };          /* 2^14 = 16384 */
        seq_t dp[MASKS];
        seq_t next[MASKS];
        size_t m, i;
        /* initialise dp */
        for (m = 0; m < MASKS; ++m) dp[m] = 0;
        dp[0] = (seq_t)1;

        idx_t cols = g + n;                 /* number of columns in B */
        for (idx_t col = 0; col < cols; ++col) {
            /* copy current state */
            for (m = 0; m < MASKS; ++m) next[m] = dp[m];

            /* add assignments to an unused row */
            for (m = 0; m < MASKS; ++m) {
                seq_t ways = dp[m];
                if (!ways) continue;
                for (idx_t r = 0; r < g; ++r) {
                    if (m & (size_t)(1U << (unsigned int)r)) continue;
                    if (col >= r && col <= r + n) {
                        size_t newmask = m | (size_t)(1U << (unsigned int)r);
                        next[newmask] = next[newmask] + ways;
                    }
                }
            }
            /* prepare for next column */
            for (m = 0; m < MASKS; ++m) dp[m] = next[m];
        }
        return dp[MASKS - 1];
    }

    /* Polynomial evaluation for n ≥ 12. */
    static const long long coeff[15] = {
        1LL,
        -77LL,
        3094LL,
        -83083LL,
        1637636LL,
        -24785761LL,
        294696402LL,
        -2779448529LL,
        20797459683LL,
        -122389753486LL,
        555826054784LL,
        -1883902028008LL,
        4494445040176LL,
        -6742111050752LL,
        4789534153984LL
    };

    seq_t res = (seq_t)coeff[0];
    for (int i = 1; i < 15; ++i) {
        res = res * (seq_t)n;
        if (coeff[i] >= 0)
            res = res + (seq_t)coeff[i];
        else
            res = res - (seq_t)(-coeff[i]);
    }
    return res;
}

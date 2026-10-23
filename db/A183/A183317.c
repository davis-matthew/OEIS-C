/*
 * A183317  Number of nX5 binary arrays with an element zero only if there are
 *          an even number of ones to its left and an even number of ones
 *          above it.
 * https://oeis.org/A183317
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n×5 binary arrays where a zero may appear only
 *            * when there is an even number of ones to its left in the same
 *            row * and an even number of ones above it in the same column. *
 *            Computed by dynamic programming over column‑parity states, *
 *            using a 32×32 transition matrix
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* Number of column‑parity states for 5 columns: 2^5 = 32. */
enum { STATES = 32 };

seq_t a(idx_t n)
{
    /* n = 0 is outside the OEIS offset; return 0 safely. */
    if (n == 0) return (seq_t)0;

    /* Transition matrix M[p][q] = number of valid rows that
       transform previous column parity state p into state q. */
    static seq_t M[STATES][STATES];
    static bool init = false;

    if (!init) {
        /* Fill with zeros */
        for (int i = 0; i < STATES; ++i)
            for (int j = 0; j < STATES; ++j)
                M[i][j] = (seq_t)0;

        /* Enumerate all previous states (p) and all row patterns (r). */
        for (int p = 0; p < STATES; ++p) {
            for (int r = 0; r < STATES; ++r) {
                bool ok = true;
                int left_parity = 0;  /* parity of ones to the left in the row */

                /* Check each column within the row. */
                for (int c = 0; c < 5; ++c) {
                    int bit = (r >> c) & 1;      /* current cell value */

                    if (bit == 0) {
                        /* Zero allowed only if left_parity and above parity are even. */
                        int above_parity = (p >> c) & 1;
                        if (left_parity != 0 || above_parity != 0) {
                            ok = false;
                            break;
                        }
                    }
                    /* Update left parity after processing this column. */
                    left_parity ^= bit;
                }

                if (ok) {
                    int q = p ^ r;              /* new column parity after adding this row */
                    M[p][q] = M[p][q] + (seq_t)1;
                }
            }
        }
        init = true;
    }

    /* Dynamic programming over rows. dp[s] = number of ways to reach state s
       after processing current number of rows. */
    seq_t dp[STATES];
    seq_t ndp[STATES];

    /* Initialize: before any rows, all column parities are zero. */
    for (int i = 0; i < STATES; ++i) dp[i] = (seq_t)0;
    dp[0] = (seq_t)1;

    /* Process n rows iteratively. */
    for (idx_t step = 0; step < n; ++step) {
        /* Zero the new state array. */
        for (int i = 0; i < STATES; ++i) ndp[i] = (seq_t)0;

        /* Transition: dp * M -> ndp */
        for (int p = 0; p < STATES; ++p) {
            if (dp[p] == (seq_t)0) continue;
            for (int q = 0; q < STATES; ++q) {
                if (M[p][q] == (seq_t)0) continue;
                ndp[q] = ndp[q] + (dp[p] * M[p][q]);
            }
        }

        /* Copy ndp back to dp for next iteration. */
        for (int i = 0; i < STATES; ++i) dp[i] = ndp[i];
    }

    /* The total number of arrays is the sum over all final states. */
    seq_t total = (seq_t)0;
    for (int i = 0; i < STATES; ++i)
        total = total + dp[i];

    return total;
}

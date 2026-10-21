/*
 * A055422  Number of points in N^n of norm <= 7.
 * https://oeis.org/A055422
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of n-tuples of nonnegative integers whose
 *            Euclidean norm (sqrt of sum of squares) is <= 7, i.e. number of
 *            ways to write x₁² + x₂² + … + xₙ² ≤ 49 with each xᵢ ≥ 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 16, u64 n <= 42, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Squares of integers 0..7 (0² … 7²) */
    static const seq_t sq[8] = {0, 1, 4, 9, 16, 25, 36, 49};

    /* DP arrays for sums 0..49 */
    seq_t dp[50] = {0};
    seq_t next[50] = {0};

    /* Start with zero dimensions: only the empty tuple, sum 0 */
    dp[0] = (seq_t)1;

    /* Process each coordinate */
    for (idx_t dim = 0; dim < n; ++dim) {
        /* Reset next array */
        for (int k = 0; k <= 49; ++k) {
            next[k] = (seq_t)0;
        }
        /* For each possible square value add contributions */
        for (int s = 0; s < 8; ++s) {
            seq_t val = sq[s];
            /* Only iterate where remaining sum is nonnegative */
            for (int k = (int)val; k <= 49; ++k) {
                next[k] += dp[k - (int)val];
            }
        }
        /* Swap arrays: dp = next */
        for (int k = 0; k <= 49; ++k) {
            dp[k] = next[k];
        }
    }

    /* Sum all counts for sums up to 49 */
    seq_t total = (seq_t)0;
    for (int k = 0; k <= 49; ++k) {
        total += dp[k];
    }

    return total;
}

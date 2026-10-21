/*
 * A060080  Scaled sums of squares.
 * https://oeis.org/A060080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n + 1)*(2*n + 1)/6 * ((n-1)!)^2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 12, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For the defined offset, n should be ≥ 1; return 0 if less. */
    if (n == 0) {
        return 0;
    }

    /* Compute (n-1)! */
    seq_t fact = 1;
    for (idx_t i = 2; i < n; ++i) {
        fact *= (seq_t)i;
    }

    /* (n-1)! squared */
    seq_t fact_sq = fact * fact;

    /* Sum of squares: n*(n+1)*(2n+1)/6 */
    seq_t sum_sq = (seq_t)n * (seq_t)(n + 1);
    sum_sq *= (seq_t)(2 * n + 1);
    sum_sq /= 6;

    /* Final result */
    return fact_sq * sum_sq;
}

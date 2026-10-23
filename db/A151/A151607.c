/*
 * A151607  Number of permutations of 6 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 2 adjacent element pairs in
 *          decreasing order.
 * https://oeis.org/A151607
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n * (3*7^n - 36*n) for n > 1, a(1) = 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for the offset term */
    if (n <= 1)
        return (seq_t)0;

    /* Compute 7^n iteratively to avoid casting the exponent into seq_t */
    seq_t pow7 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        pow7 = pow7 * (seq_t)7;
    }

    /* Compute the factor 3*7^n - 36*n */
    seq_t factor = (seq_t)3 * pow7;
    /* 36*n fits into idx_t (unsigned 64‑bit) for the intended range */
    idx_t sub = (idx_t)36 * n;
    factor = factor - (seq_t)sub;

    /* Final result: n * factor */
    return (seq_t)n * factor;
}

/*
 * A151597  Number of permutations of 4 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 2 adjacent element pairs in
 *          decreasing order.
 * https://oeis.org/A151597
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = n * (2*5^n - 16*n)   for n > 1
 *            a(1) = 0
 * Implementation uses a simple loop to compute 5^n.
 */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    /* compute 5^n */
    seq_t pow5 = 1;
    for (idx_t i = 1; i <= n; ++i)
        pow5 *= (seq_t)5;

    /* term = 2*5^n - 16*n */
    seq_t term = pow5 * 2 - ((seq_t)16 * (seq_t)n);

    /* a(n) = n * term */
    return (seq_t)n * term;
}

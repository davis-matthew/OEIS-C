/*
 * A151590  Number of permutations of 3 indistinguishable copies of 1..n
 *          arranged in a circle with exactly 2 adjacent element pairs in
 *          decreasing order.
 * https://oeis.org/A151590
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*((3/2)*4^n - 9*n) for n > 1, a(1) = 0
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 1)
        return 0;

    seq_t n_t = (seq_t)n;
    seq_t pow4 = 1;                /* 4^(n-1) */
    for (idx_t i = 1; i < n; ++i)
        pow4 *= (seq_t)4;

    /* term = 6 * n * 4^(n-1) */
    seq_t term = (seq_t)6 * pow4;
    term *= n_t;

    /* term2 = 9 * n^2 */
    seq_t term2 = (seq_t)9 * n_t * n_t;

    return term - term2;
}

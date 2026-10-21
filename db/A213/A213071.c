/*
 * A213071  3*n*(9n + 2)*(18n - 1), where n runs through the odd numbers 1, 3,
 *          5,...
 * https://oeis.org/A213071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*(2n-1)*(9*(2n-1)+2)*(18*(2n-1)-1)
 * Range:     u8 none, u16 n <= 3, u32 n <= 103, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert index to seq_t first */
    seq_t m = (seq_t)2 * n - (seq_t)1;                           /* m = 2*n - 1 (odd value) */
    seq_t term = m * ((((seq_t)9) * m) + (seq_t)2);
    term *= ((((seq_t)18) * m) - (seq_t)1);
    term *= (seq_t)3;
    return term;
}

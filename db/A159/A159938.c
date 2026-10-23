/*
 * A159938  The number of homogeneous trisubstituted linear alkanes.
 * https://oeis.org/A159938
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/3)*(2*n^3 - 9*n^2 + 19*n - 12)
 * Range:     u8 n <= 6, u16 n <= 33, u32 n <= 1330, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;

    seq_t nn = (seq_t)n;
    /* Compute numerator: 2*n^3 - 9*n^2 + 19*n - 12 */
    seq_t term1 = (seq_t)2 * nn * nn * nn;      /* 2*n^3   */
    seq_t term2 = (seq_t)9 * nn * nn;           /* 9*n^2   */
    seq_t term3 = (seq_t)19 * nn;              /* 19*n    */
    seq_t num = term1 - term2 + term3 - (seq_t)12;

    return num / (seq_t)3;
}

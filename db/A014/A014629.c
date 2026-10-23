/*
 * A014629  Number of segments created by diagonals of n-gon.
 * https://oeis.org/A014629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^4+2*n^3+5*n^2+4*n-12)/12
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1. For n == 0 a dummy
       value is returned because the offset starts at 1. */
    if (n == 0) return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;            /* n^2   */
    seq_t n3 = n2 * nn;            /* n^3   */
    seq_t n4 = n3 * nn;            /* n^4   */
    seq_t num = n4 + 2 * n3 + 5 * n2 + 4 * nn - 12;
    return num / 12;
}

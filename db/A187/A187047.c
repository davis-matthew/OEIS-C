/*
 * A187047  Number of 3-step one or two space at a time bishop's tours on an n
 *          X n board summed over all starting positions.
 * https://oeis.org/A187047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=0, a(2)=0, a(3)=28, a(n)=56*n^2-280*n+360 for n>3
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n <= 2) return 0;
    if (n == 3) return 28;
    /* n > 3 */
    seq_t tn = (seq_t)n;
    return 56 * tn * tn - 280 * tn + 360;
}

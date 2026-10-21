/*
 * A211850  Number of nonnegative integer arrays of length 2n+5 with new
 *          values 0 upwards introduced in order, no three adjacent elements
 *          all unequal, and containing the value n+1.
 * https://oeis.org/A211850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (14*n^3 + 81*n^2 + 163*n + 120)/6
 * Range:     u8 none, u16 n <= 14, u32 n <= 210, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1 per OEIS offset */
    seq_t nn  = (seq_t)n;                /* cast to seq_t */
    seq_t n2  = nn * nn;                 /* n^2 */
    seq_t n3  = n2 * nn;                 /* n^3 */
    seq_t num = 14 * n3 + 81 * n2 + 163 * nn + 120;
    return num / 6;
}

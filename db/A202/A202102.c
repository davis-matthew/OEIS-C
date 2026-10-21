/*
 * A202102  Number of points matched in largest non-crossing matching of
 *          2n=w+b points in the plane (w white, b black) where w, b are both
 *          even.
 * https://oeis.org/A202102
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(1)=2, a(n)=2*(n-1) for n>1
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)2;
    return (seq_t)2 * (((seq_t)n) - 1);
}

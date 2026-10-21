/*
 * A063551  Maximal number of pairs of elements of an n-set S in the plane
 *          that can be separated from S by a straight line.
 * https://oeis.org/A063551
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, hard
 * Formula:   a(n) = floor(3*n/2) for n>=5, a(4)=3 Maximal number of pairs of
 *            elements of an n-set S in the plane that can be separated from S
 *            by a straight line
 * Range:     u8 n <= 10, u16 n <= 10, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 4)
        return (seq_t)3;
    return (seq_t)3 * (seq_t)n / (seq_t)2;
}

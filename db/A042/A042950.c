/*
 * A042950  Row sums of the Lucas triangle A029635.
 * https://oeis.org/A042950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=2, a(n)=3*2^(n-1) for n>=1; equivalently a(n)=2*a(n-1) for
 *            n>1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)2;
    seq_t result = (seq_t)3;
    for (idx_t i = 1; i < n; ++i)
        result = result * (seq_t)2;
    return result;
}

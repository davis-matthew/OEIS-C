/*
 * A083066  5th row of number array A083064.
 * https://oeis.org/A083066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1)-1 with a(0)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t val = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        val = (seq_t)6 * val - (seq_t)1;
    }
    return val;
}

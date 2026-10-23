/*
 * A117616  a(0)=0, a(n)=4a(n-1)+2 for n odd, a(n)=4a(n-1) for n even.
 * https://oeis.org/A117616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1)+2 if n is odd, 4*a(n-1) if n is even, with
 *            a(0)=0
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t value = 0;
    for (idx_t i = 1; i <= n; ++i)
    {
        if (i & 1)
            value = value * 4 + 2;
        else
            value = value * 4;
    }
    return value;
}

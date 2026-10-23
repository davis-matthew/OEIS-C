/*
 * A108355  Expansion of (1+2x^2)/((1-x)^2(1+x^2)^2).
 * https://oeis.org/A108355
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (-cos(pi*n/2)/4)+(1-n)*sin(pi*n/2)/4+(3n+5)/4
 * Range:     u8 n <= 72, u16 n <= 72, u32 n <= 72, u64 n <= 72, u128 n <= 72,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t r = n & 3u;           /* n modulo 4 */
    seq_t ns = (seq_t)n;

    switch (r)
    {
    case 0u:
        return (3 * ns + (seq_t)4) / (seq_t)4;
    case 1u:
        return (ns + (seq_t)3) / (seq_t)2;
    case 2u:
        return (3 * ns + (seq_t)6) / (seq_t)4;
    case 3u:
        return ns + (seq_t)1;
    }
    return (seq_t)0; /* unreachable, keeps compiler happy */
}

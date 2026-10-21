/*
 * A085351  Expansion of g.f. (1-3*x)/((1-4*x)*(1-5*x)).
 * https://oeis.org/A085351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*5^n - 4^n, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t pow5 = 1;
    seq_t pow4 = 1;
    for (idx_t i = 0; i < n; ++i)
    {
        pow5 *= (seq_t)5;
        pow4 *= (seq_t)4;
    }
    return (seq_t)2 * pow5 - pow4;
}

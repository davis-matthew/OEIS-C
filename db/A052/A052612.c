/*
 * A052612  Expansion of e.g.f. x*(2+x)/(1-x^2).
 * https://oeis.org/A052612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n! * A000034(n) where A000034(n) = 1 if n is even, 2 if n
 *            is odd (a(0) is defined as 0). This is equivalent to: a(2n) =
 *            (2n)!, a(2n+1) = 2*(2n+1)!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u)
        return 0;
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;
    if (n & 1u)
        fact *= 2;
    return fact;
}

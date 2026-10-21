/*
 * A066023  (a(n)^7+1)/(n^7+1) is the smallest integer > 1.
 * https://oeis.org/A066023
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3 if n == 1; otherwise a(n) = n^3 (A066023)] This matches
 *            the explicit terms: 3,8,27,64,125,…
 * Range:     u8 n <= 6, u16 n <= 40, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)3;
    seq_t t = (seq_t)n;
    return t * t * t;
}

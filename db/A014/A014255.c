/*
 * A014255  Expansion of (1+2*x+3*x^2)/((1-x)*(1-x^2)^2).
 * https://oeis.org/A014255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)^2 - floor((n+1)/2)^2
 * Range:     u8 n <= 17, u16 n <= 294, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n + 1;
    seq_t q = t / 2;
    return t * t - q * q;
}

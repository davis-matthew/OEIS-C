/*
 * A117571  Expansion of (1+2*x^2)/((1-x)*(1-x^3)).
 * https://oeis.org/A117571
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 1 + floor((n+1)/3) + floor(2*n/3)
 * Range:     u8 n <= 71, u16 n <= 71, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns = (seq_t)n;
    return (seq_t)1 + (ns + (seq_t)1) / (seq_t)3 + (ns * (seq_t)2) / (seq_t)3;
}

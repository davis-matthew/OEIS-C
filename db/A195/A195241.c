/*
 * A195241  Expansion of (1-x+19*x^3-3*x^4)/(1-x)^3.
 * https://oeis.org/A195241
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*n^2-20*n+11 for n>1; a(0)=1, a(1)=2
 * Range:     u8 n <= 6, u16 n <= 91, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;
    seq_t nn = (seq_t)n;
    seq_t val = nn * nn;
    val *= (seq_t)8;
    val += (seq_t)11;
    val -= (seq_t)20 * nn;
    return val;
}

/*
 * A158416  Expansion of g.f. (1+x-x^3)/(1-x^2)^2.
 * https://oeis.org/A158416
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..floor(n/2)} binomial(k+1, n-k) Equivalent
 *            closed form: a(n) = (n even) ? n/2 + 1 : 1
 * Range:     u8 n <= 87, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if ((n & 1) == 0)
        return ((seq_t)(n >> 1)) + (seq_t)1;
    else
        return (seq_t)1;
}

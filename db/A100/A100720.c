/*
 * A100720  Expansion of g.f.: (3+x+2*x^2-2*x^3)/((1-2*x)*(1+x^2)).
 * https://oeis.org/A100720
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = [n==0] + 3*2^n - (-1)^floor((n+1)/2) (OEIS A100720)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n by repeated multiplication to avoid UB with shift */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow2 *= (seq_t)2;

    seq_t result = (seq_t)3 * pow2;

    idx_t t = (n + 1) >> 1;                /* floor((n+1)/2) */
    seq_t sign = ((t & 1) == 0) ? (seq_t)1 : (seq_t)(-1);

    result -= sign;

    if (n == 0)                             /* add the indicator [n==0] */
        result += (seq_t)1;

    return result;
}

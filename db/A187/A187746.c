/*
 * A187746  G.f.: Sum_{n>=0} (2*n+x)^n * x^n / (1 + 2*n*x + x^2)^n.
 * https://oeis.org/A187746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^2+2*n+1) * 2^(n-2) * (n-1)! for n > 1 with a(0) = 1,
 *            a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;
    if (n == (idx_t)1) return (seq_t)2;

    /* Compute the polynomial part: 2*n^2 + 2*n + 1 */
    seq_t coeff = (seq_t)1
                 + (seq_t)2 * (seq_t)n
                 + (seq_t)2 * (seq_t)n * (seq_t)n;

    /* Compute (n-1)! */
    seq_t fact = (seq_t)1;
    for (idx_t i = (idx_t)2; i <= n - (idx_t)1; ++i)
        fact *= (seq_t)i;

    /* Compute 2^(n-2) */
    seq_t pow2 = (seq_t)1;
    for (idx_t i = (idx_t)0; i < n - (idx_t)2; ++i)
        pow2 *= (seq_t)2;

    /* Final product */
    return coeff * pow2 * fact;
}

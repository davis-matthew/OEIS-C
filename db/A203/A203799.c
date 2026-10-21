/*
 * A203799  G.f.: Sum_{n>=0} (n-2*x)^n * x^n / (1 + n*x - 2*x^2)^n.
 * https://oeis.org/A203799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^2 + n - 4)/2 * (n-1)! for n > 1, a(0)=a(1)=1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two special cases directly. */
    if (n == 0 || n == 1)
        return (seq_t)1;

    /* Compute (n-1)!; loop from 2 up to n-1. */
    seq_t fact = (seq_t)1;
    for (idx_t i = 2; i < n; ++i)
        fact *= (seq_t)i;

    /* Coefficient (n^2 + n - 4)/2, always integer for n>1. */
    seq_t num   = (seq_t)n * (seq_t)n + (seq_t)n - (seq_t)4;
    seq_t coeff = num / (seq_t)2;

    return coeff * fact;
}

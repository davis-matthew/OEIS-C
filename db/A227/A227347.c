/*
 * A227347  Number of lattice points in the closed region bounded by the
 *          graphs of y = (5/6)*x^2, x = n, and y = 0, excluding points on the
 *          x-axis.
 * https://oeis.org/A227347
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{x=1..n} floor((5/6) * x^2)
 * Range:     u8 n <= 9, u16 n <= 61, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) with an O(n) loop. The sum is performed in the
   seq_t type to keep the result exact for all supported sizes. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* values below offset are irrelevant */
    seq_t sum = 0;
    for (idx_t x = 1; x <= n; ++x)        /* simple summation */
    {
        seq_t xx = (seq_t)x * (seq_t)x;   /* x^2 */
        seq_t term = ( (seq_t)5 * xx ) / 6; /* floor((5/6) * x^2) */
        sum += term;
    }
    return sum;
}

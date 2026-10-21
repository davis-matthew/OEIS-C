/*
 * A066114  a(0) = 1; for n > 0, a(n) = (n!*(3*n+1))/2.
 * https://oeis.org/A066114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n! * (3*n + 1)) / 2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the base case */
    if (n == 0)
        return (seq_t)1;

    /* Compute n! */
    seq_t fact = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    /* Multiply by (3*n + 1) and divide by 2 */
    seq_t mult = (seq_t)(3 * n + 1);
    fact *= mult;
    fact /= (seq_t)2;

    return fact;
}

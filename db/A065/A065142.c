/*
 * A065142  a(n) = 2^n*(n+1)*(3*n)!.
 * https://oeis.org/A065142
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n * (n+1) * (3*n)!
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t two_pow = 1;
    for (idx_t i = 0; i < n; ++i)
        two_pow *= 2;

    /* Compute (3*n)! */
    idx_t m = 3 * n;
    seq_t fact = 1;
    for (idx_t i = 2; i <= m; ++i)
        fact *= (seq_t)i;

    /* Combine factors: (n+1) * fact * two_pow */
    seq_t result = (seq_t)n + 1;
    result *= fact;
    result *= two_pow;

    return result;
}

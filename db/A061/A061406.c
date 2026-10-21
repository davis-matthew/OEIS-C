/*
 * A061406  Denominators of coefficients of odd powers of x of the expansion
 *          of Bessel function J_7(x).
 * https://oeis.org/A061406
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n+7) * n! * (n+7)!
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute n!   */
    seq_t fact_n = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact_n *= (seq_t)i;

    /* compute (n+7)! */
    seq_t fact_n7 = 1;
    idx_t limit = n + 7;
    for (idx_t i = 2; i <= limit; ++i)
        fact_n7 *= (seq_t)i;

    /* compute 2^(2*n+7) */
    seq_t pow2 = 1;
    idx_t exp = 2 * n + 7;
    for (idx_t i = 0; i < exp; ++i)
        pow2 *= 2;

    return pow2 * fact_n * fact_n7;
}

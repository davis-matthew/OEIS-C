/*
 * A061404  Denominators of coefficients of odd powers of x of the expansion
 *          of Bessel function J_5(x).
 * https://oeis.org/A061404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^(2*n+5) * n! * (n+5)! (offset 0)
 * Range:     u8 none, u16 n <= 0, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    /* multiply by 2^(2*n+5) */
    idx_t exp = (n << 1) + 5; /* 2*n + 5 */
    for (idx_t i = 0; i < exp; ++i)
        result *= (seq_t)2;

    /* multiply by (n+5)! */
    for (idx_t i = 1; i <= n + 5; ++i)
        result *= (seq_t)i;

    /* multiply by n! again */
    for (idx_t i = 1; i <= n; ++i)
        result *= (seq_t)i;

    return result;
}

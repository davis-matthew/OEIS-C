/*
 * A052594  E.g.f. x(1+x-2x^2)/(1-2x).
 * https://oeis.org/A052594
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, a(2) = 6, a(n) = 2^(n-1) * n! (for n ≥ 3)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    if (n == 1)
        return (seq_t)1;
    if (n == 2)
        return (seq_t)6;

    /* Compute n! */
    seq_t res = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i)
        res *= (seq_t)i;

    /* Multiply by 2^(n-1) using repeated left shift */
    for (idx_t i = 1; i <= n - 1; ++i)
        res <<= 1;

    return res;
}

/*
 * A062195  Sixth (unsigned) column sequence of triangle A062139 (generalized
 *          a=2 Laguerre).
 * https://oeis.org/A062195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+5)! * binomial(n+7, 7) / 5!
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where binomial(n, k) = C(n, k) is computed multiplicatively. */

seq_t a(idx_t n)
{
    /* Compute (n+5)! */
    idx_t m = n + 5;
    seq_t fact = 1;
    for (idx_t i = 2; i <= m; ++i)
        fact *= i;

    /* Divide by 5! (120) – the division is exact for all n >= 0 */
    fact /= 120;

    /* Compute binomial(n+7, 7) multiplicatively.
       We use small integer division that is exact at every step. */
    idx_t m2 = n + 7;
    seq_t binom = 1;
    for (idx_t k = 1; k <= 7; ++k)
        binom = binom * (m2 - k + 1) / k;

    /* Final result */
    return fact * binom;
}

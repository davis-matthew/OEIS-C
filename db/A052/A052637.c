/*
 * A052637  E.g.f. 3x(1+x-x^2)/(1-x).
 * https://oeis.org/A052637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0, 3, 12 for n = 0,1,2, and for n ≥ 3 a(n) = 3 * n!
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 3;
    if (n == 2) return 12;
    /* n ≥ 3 : compute n! and multiply by 3 */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;
    return 3 * fact;
}

/*
 * A052628  Expansion of e.g.f. (2+x^3-x^4)/(1-x).
 * https://oeis.org/A052628
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 2, a(1) = 2, a(2) = 4, a(3) = 18, a(n) = 2 · n! for all n
 *            ≠ 3 (derived from the EGF (2 + x³ – x⁴)/(1–x).)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* special case n == 3 */
    if (n == 3)
        return (seq_t)18;

    /* compute n! */
    seq_t fact = 1;
    for (idx_t i = 1; i <= n; ++i)
        fact *= (seq_t)i;

    return (seq_t)2 * fact;
}

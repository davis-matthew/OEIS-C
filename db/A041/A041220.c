/*
 * A041220  Numerators of continued fraction convergents to sqrt(122).
 * https://oeis.org/A041220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 22*a(n-1) + a(n-2); a(0)=11, a(1)=243
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)11;
    if (n == 1)
        return (seq_t)243;

    seq_t prev2 = (seq_t)11;
    seq_t prev1 = (seq_t)243;
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i)
    {
        cur = (seq_t)22 * prev1 + prev2;
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1;
}

/*
 * A153647  a(n) = 3^n*(n + 2)!.
 * https://oeis.org/A153647
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n*(n + 2)!
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;
    seq_t res = (seq_t)2;
    for (idx_t i = 1; i <= n; ++i)
    {
        res *= (seq_t)3;
        res *= (seq_t)(i + 2);
    }
    return res;
}

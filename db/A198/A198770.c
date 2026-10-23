/*
 * A198770  a(n) = 11*5^n-1.
 * https://oeis.org/A198770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11*5^n-1
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t res = 10;                 /* a(0) */
    for (idx_t i = 1; i <= n; ++i)
        res = res * (seq_t)5 + 4;   /* a(n) = 5*a(n-1)+4 */
    return res;
}

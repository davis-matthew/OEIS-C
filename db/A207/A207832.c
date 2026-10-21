/*
 * A207832  Numbers x such that 20*x^2 + 1 is a perfect square.
 * https://oeis.org/A207832
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - a(n-2), a(0) = 0, a(1) = 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 31,
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
        return (seq_t)2;
    seq_t a0 = (seq_t)0;          /* a(n-2) */
    seq_t a1 = (seq_t)2;          /* a(n-1) */
    seq_t a2;                     /* a(n) */
    for (idx_t i = 2; i <= n; ++i) {
        a2 = ((seq_t)18 * a1) - a0;
        a0 = a1;
        a1 = a2;
    }
    return a1;
}

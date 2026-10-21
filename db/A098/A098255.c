/*
 * A098255  Chebyshev polynomials S(n,443) + S(n-1,443) with Diophantine
 *          property.
 * https://oeis.org/A098255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 443*a(n-1) - a(n-2), a(0) = 1, a(1) = 444
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)444;
    seq_t prevprev = (seq_t)1;
    seq_t prev     = (seq_t)444;
    seq_t cur;
    for (idx_t i = 2; i <= n; ++i)
    {
        cur       = (seq_t)443 * prev - prevprev;
        prevprev  = prev;
        prev      = cur;
    }
    return cur;
}

/*
 * A077235  Bisection (odd part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077235
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-1) - a(n-2), a(0) = 5, a(1) = 16
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 32, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)5;
    if (n == 1) return (seq_t)16;
    seq_t a0 = (seq_t)5;
    seq_t a1 = (seq_t)16;
    seq_t an = a1;
    for (idx_t i = 2; i <= n; ++i)
    {
        an = (4 * a1) - a0;
        a0 = a1;
        a1 = an;
    }
    return an;
}

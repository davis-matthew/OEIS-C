/*
 * A077251  Bisection (even part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077251
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 12, a(n) = 10*a(n-1) – a(n-2) for n >= 2
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)12;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)12;
    seq_t an;

    for (idx_t i = 2; i <= n; ++i) {
        an = (seq_t)10 * a1 - a0;
        a0 = a1;
        a1 = an;
    }

    return a1;
}

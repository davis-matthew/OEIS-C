/*
 * A077250  Bisection (odd part) of Chebyshev sequence with Diophantine
 *          property.
 * https://oeis.org/A077250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - a(n-2); a(0)=11, a(1)=103
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)11;
    if (n == 1) return (seq_t)103;

    seq_t a0 = (seq_t)11; /* a(0) */
    seq_t a1 = (seq_t)103;/* a(1) */
    seq_t a2;

    for (idx_t i = 2; i <= n; ++i) {
        a2 = (seq_t)10 * a1 - a0; /* a(i) = 10*a(i-1) - a(i-2) */
        a0 = a1;
        a1 = a2;
    }
    return a1; /* a(n) */
}

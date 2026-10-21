/*
 * A098245  Chebyshev polynomials S(n,227).
 * https://oeis.org/A098245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 227*a(n-1) - a(n-2), a(0)=1, a(1)=227
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)227;

    seq_t a0 = (seq_t)1;
    seq_t a1 = (seq_t)227;

    for (idx_t i = 2; i <= n; ++i) {
        seq_t a2 = (seq_t)227 * a1 - a0;
        a0 = a1;
        a1 = a2;
    }

    return a1;
}

/*
 * A098249  Chebyshev polynomials S(n,291) + S(n-1,291) with Diophantine
 *          property.
 * https://oeis.org/A098249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 292, a(n) = 291*a(n-1) - a(n-2) for n >= 2
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 7, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)292;
    seq_t a_nm2 = (seq_t)1;          /* a(n-2) */
    seq_t a_nm1 = (seq_t)292;        /* a(n-1) */
    idx_t i;
    for (i = 2; i <= n; ++i) {
        seq_t cur = (seq_t)291 * a_nm1 - a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}

/*
 * A099158  a(n) = 5^(n-1) * U(n-1, 7/5) where U is the Chebyshev polynomial
 *          of the second kind.
 * https://oeis.org/A099158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 14*a(n-1) - 25*a(n-2), with a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* handle the two known base cases */
    if (n == (idx_t)0) return (seq_t)0;
    if (n == (idx_t)1) return (seq_t)1;

    /* iterate from 2 up to n using the recurrence. */
    seq_t a_nm2 = (seq_t)0;          /* a(n-2) */
    seq_t a_nm1 = (seq_t)1;          /* a(n-1) */
    seq_t a_n   = (seq_t)0;          /* a(n) */

    for (idx_t i = (idx_t)2; i <= n; ++i) {
        a_n = (seq_t)14 * a_nm1 - (seq_t)25 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }

    return a_nm1;
}

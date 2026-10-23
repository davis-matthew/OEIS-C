/*
 * A177373  a(n) = 2*n*a(n-1) if the parity of the ratio a(n-1)/a(n-2) is odd,
 *          otherwise (for even parity) a(n) = (2n-1)*a(n-1).
 * https://oeis.org/A177373
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, and for n≥2 *
 *            a(n)=2*(a(n-1)+(n-1)*(2*n-3)*a(n-2))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* base cases */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    /* iterate from the bottom up */
    seq_t a_nm2 = (seq_t)1;  /* a(0) */
    seq_t a_nm1 = (seq_t)2;  /* a(1) */
    for (idx_t k = 2; k <= n; ++k) {
        seq_t mult1 = (seq_t)(k - 1);
        seq_t mult2 = (seq_t)(2 * k - 3);
        seq_t inner = a_nm1 + ((a_nm2 * mult1) * mult2);
        seq_t curr  = inner * (seq_t)2;
        a_nm2 = a_nm1;
        a_nm1 = curr;
    }
    return a_nm1;
}

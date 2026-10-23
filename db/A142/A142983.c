/*
 * A142983  a(1) = 1, a(2) = 2, a(n+2) = 2*a(n+1) + (n + 1)*(n + 2)*a(n).
 * https://oeis.org/A142983
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1) = 1, a(2) = 2, a(n+2) = 2*a(n+1) + (n+1)*(n+2)*a(n) (OEIS
 *            A142983)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;

    /* a(n) depends only on the two previous terms; compute iteratively. */
    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)2;   /* a(2) */
    seq_t a_n;

    for (idx_t i = 1; i <= n - 2; ++i) {
        seq_t coeff = (seq_t)(i + 1) * (seq_t)(i + 2);   /* (i+1)*(i+2) */
        a_n = 2 * a_nm1 + coeff * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

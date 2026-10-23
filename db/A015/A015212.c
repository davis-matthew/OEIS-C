/*
 * A015212  Sum of Gaussian binomial coefficients for q=21.
 * https://oeis.org/A015212
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1, a(1) = 2, a(n) = 2*a(n-1) + a(n-2)*((21^(n-1)) - 1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)2;

    seq_t a_nm2 = (seq_t)1;        /* a(n-2) */
    seq_t a_nm1 = (seq_t)2;        /* a(n-1) */
    seq_t pow21 = (seq_t)21;       /* 21^(i-1) for current i */

    for (idx_t i = 2; i <= n; ++i) {
        seq_t term = a_nm1 * (seq_t)2 + a_nm2 * (pow21 - (seq_t)1);
        a_nm2 = a_nm1;
        a_nm1 = term;
        pow21 = pow21 * (seq_t)21; /* advance to 21^i */
    }
    return a_nm1;
}

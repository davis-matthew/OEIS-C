/*
 * A041059  Denominators of continued fraction convergents to sqrt(35).
 * https://oeis.org/A041059
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(0)=1; for n>=1, a(n) = ((n%2==1)?1:10)*a(n-1)+a(n-2)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    seq_t q_nm2 = 0;   /* a(-1) */
    seq_t q_nm1 = 1;   /* a(0)  */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t coeff = (i & 1U) ? 1 : 10;          /* 1 for odd i, 10 for even i */
        seq_t q_n = coeff * q_nm1 + q_nm2;
        q_nm2 = q_nm1;
        q_nm1 = q_n;
    }

    return q_nm1;
}

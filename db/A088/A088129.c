/*
 * A088129  Expansion of e.g.f. sinh(x)/(1-x)^2.
 * https://oeis.org/A088129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=0, a(1)=1; for n≥2:
 *            a(n)=2*n*a(n-1)-n*(n-1)*a(n-2)+(n%2?1:0)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    seq_t a_nm2 = (seq_t)0;         /* a(n-2) */
    seq_t a_nm1 = (seq_t)1;         /* a(n-1) */
    seq_t a_n   = (seq_t)0;         /* a(n) */

    for (idx_t i = 2; i <= n; ++i) {
        /* term1 = 2*i * a(n-1) */
        seq_t term1 = (seq_t)(2 * i) * a_nm1;
        /* term2 = i*(i-1) * a(n-2) */
        seq_t term2 = (seq_t)(i * (i - 1)) * a_nm2;

        if (i & 1) /* odd n: add 1 */
            a_n = term1 - term2 + (seq_t)1;
        else
            a_n = term1 - term2;

        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

/*
 * A142989  a(1) = 1, a(2) = 5, a(n+2) = 5*a(n+1)+(n+1)*(n+3)*a(n).
 * https://oeis.org/A142989
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(1)=1, a(2)=5, a(n+2)=5*a(n+1)+(n+1)(n+3)*a(n) (n≥1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Handle the two initial terms directly */
    if (n == 1) return (seq_t)1U;
    if (n == 2) return (seq_t)5U;

    seq_t a_nm2 = (seq_t)1U;   /* a(n-2)  */
    seq_t a_nm1 = (seq_t)5U;   /* a(n-1)  */
    seq_t a_n;                 /* a(n)    */

    for (idx_t i = 3; i <= n; ++i) {
        /* multiplier = (i-1)*(i+1)  */
        seq_t mult = (seq_t)(i - 1) * (seq_t)(i + 1);
        a_n = 5 * a_nm1 + mult * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_nm1;
}

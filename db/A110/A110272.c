/*
 * A110272  a(n) = Pell(n)^3.
 * https://oeis.org/A110272
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Pell(n)^3 where Pell(0)=0, Pell(1)=1 and
 *            Pell(k)=2*Pell(k-1)+Pell(k-2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    if (n == 1) return 1;           /* Pell(1)^3 = 1 */
    seq_t p_nm2 = 0;                 /* Pell(n-2)  */
    seq_t p_nm1 = 1;                 /* Pell(n-1)  */
    seq_t p_n;                       /* Pell(n)    */
    for (idx_t i = 2; i <= n; ++i) {
        p_n = 2 * p_nm1 + p_nm2;
        p_nm2 = p_nm1;
        p_nm1 = p_n;
    }
    seq_t res = p_n;
    res = res * res * res;           /* cube */
    return res;
}

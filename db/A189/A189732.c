/*
 * A189732  a(1)=1, a(2)=5, a(n) = a(n-1) + 5*a(n-2).
 * https://oeis.org/A189732
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=1, a(2)=5, a(n)=a(n-1)+5*a(n-2)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 5;
    seq_t a_nm2 = 1;   /* a(n-2) */
    seq_t a_nm1 = 5;   /* a(n-1) */
    seq_t a_n   = 0;   /* a(n)   */
    for (idx_t i = 3; i <= n; ++i) {
        a_n   = a_nm1 + 5 * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

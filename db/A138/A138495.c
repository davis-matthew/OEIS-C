/*
 * A138495  First differences of A138477.
 * https://oeis.org/A138495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 2*a(n-3) + 4*a(n-4) initial values: a(0) = 0,
 *            a(1) = 1, a(2) = 1, a(3) = 1
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 34, u64 n <= 66, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0u) return (seq_t)0;
    if (n == 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)1;
    if (n == 3u) return (seq_t)1;

    /* Iterative computation */
    seq_t a_nm4 = (seq_t)0,  /* a(n-4)  */
           a_nm3 = (seq_t)1,  /* a(n-3)  */
           a_nm2 = (seq_t)1,  /* a(n-2)  */
           a_nm1 = (seq_t)1;  /* a(n-1)  */
    seq_t a_cur;

    for (idx_t i = 4; i <= n; ++i) {
        a_cur = a_nm1 + (a_nm3 << 1) + (a_nm4 << 2); /* a(n-1)+2*a(n-3)+4*a(n-4) */
        a_nm4 = a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_cur;
    }

    return a_nm1;
}

/*
 * A130816  a(1) = 2, a(2) = 2, a(3) = 1, a(n) = a(n-3) + floor(a(n-2)/2) for
 *          n >= 4.
 * https://oeis.org/A130816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1)=2, a(2)=2, a(3)=1, a(n)=a(n-3)+floor(a(n-2)/2) for n>=4
 * Range:     u8 n <= 37, u16 n <= 73, u32 n <= 145, u64 n <= 290,
 *            u128 n <= 580, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 1) return (seq_t)2;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)1;

    /* Maintain the last three terms: a(n-3), a(n-2), a(n-1) */
    seq_t a_nm3 = (seq_t)2;   /* a(1)   */
    seq_t a_nm2 = (seq_t)2;   /* a(2)   */
    seq_t a_nm1 = (seq_t)1;   /* a(3)   */

    for (idx_t k = 4; k <= n; ++k) {
        seq_t val = a_nm3 + (a_nm2 >> 1); /* floor(a(n-2)/2) is shift right */
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = val;
    }
    return a_nm1;
}

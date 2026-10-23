/*
 * A180195  a(n) = (-1)^n*Sum((-1)^j*b(j), j=1..n), where b(n)=(n-1)!*(n^2 - n
 *          + 1) = A001564(n-1) (n>=1).
 * https://oeis.org/A180195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*a(n-1) + 4*a(n-2) - (n-2)*a(n-3) for n ≥ 4 Initial
 *            values: a(1)=1, a(2)=2, a(3)=12
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;          /* outside offset – return 0 */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)2;
    if (n == 3) return (seq_t)12;

    seq_t a_nm3 = (seq_t)1;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)2;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)12;  /* a(n-1) */
    seq_t a_n   = 0;

    for (idx_t i = 4; i <= n; ++i) {
        a_n = (seq_t)(i + 1) * a_nm1 + (seq_t)4 * a_nm2 - (seq_t)(i - 2) * a_nm3;
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

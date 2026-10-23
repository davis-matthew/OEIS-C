/*
 * A116423  Binomial transform of A006053.
 * https://oeis.org/A116423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 3*a(n-2) - a(n-3), a(1) = 0, a(2) = 1, a(3) =
 *            3 (valid for n >= 1, arbitrary order of calls)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is 1-based; behaviour for n < 1 is irrelevant */
    if (n == 0) return (seq_t)0;

    /* handle the first three terms directly */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)3;

    /* use the recurrence for n >= 4 */
    seq_t a_nm3 = (seq_t)0;   /* a(n-3) */
    seq_t a_nm2 = (seq_t)1;   /* a(n-2) */
    seq_t a_nm1 = (seq_t)3;   /* a(n-1) */
    seq_t cur;

    for (idx_t i = 4; i <= n; ++i) {
        cur = (seq_t)(4 * a_nm1 - 3 * a_nm2 - a_nm3);
        a_nm3 = a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = cur;
    }
    return a_nm1;
}

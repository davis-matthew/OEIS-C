/*
 * A067078  a(1) = 1, a(2) = 2, a(n) = (n-1)*a(n-1) - (n-2)*a(n-2).
 * https://oeis.org/A067078
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n-1)*a(n-1) - (n-2)*a(n-2) with initial values a(1) = 1,
 *            a(2) = 2
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 14, u64 n <= 22, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Handle the first two terms explicitly. */
    if (n <= 1u) return (seq_t)1;
    if (n == 2u) return (seq_t)2;

    /* Previous two values: a(n-2) and a(n-1). */
    seq_t a_nm2 = (seq_t)1;   /* a(1) */
    seq_t a_nm1 = (seq_t)2;   /* a(2) */
    seq_t a_n = 0;

    /* Iteratively apply the recurrence for i = 3 .. n. */
    for (idx_t i = 3u; i <= n; ++i) {
        a_n = ((seq_t)(i - 1u)) * a_nm1 - ((seq_t)(i - 2u)) * a_nm2;
        a_nm2 = a_nm1;
        a_nm1 = a_n;
    }
    return a_n;
}

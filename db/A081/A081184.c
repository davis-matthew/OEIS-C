/*
 * A081184  7th binomial transform of (0,1,0,2,0,4,0,8,0,16,...).
 * https://oeis.org/A081184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 14*a(n-1) - 47*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    seq_t a0 = 0;         /* a(0) */
    seq_t a1 = 1;         /* a(1) */
    seq_t an = 0;
    for (idx_t i = 2; i <= n; ++i) {
        an = a1 * 14 - a0 * 47;
        a0 = a1;
        a1 = an;
    }
    return a1;
}

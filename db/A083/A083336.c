/*
 * A083336  a(n) = 4*a(n-2) - a(n-4).
 * https://oeis.org/A083336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*a(n-2) - a(n-4) ; a(0)=3, a(1)=3, a(2)=9, a(3)=12
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)3;
    if (n == 2) return (seq_t)9;
    if (n == 3) return (seq_t)12;
    /* Only need the last four values to compute the next one */
    seq_t v[4];
    v[0] = (seq_t)3; /* a(0) */
    v[1] = (seq_t)3; /* a(1) */
    v[2] = (seq_t)9; /* a(2) */
    v[3] = (seq_t)12;/* a(3) */
    for (idx_t i = 4; i <= n; ++i) {
        seq_t val = v[(i - 2) & 3] * 4 - v[(i - 4) & 3];
        v[i & 3] = val;
    }
    return v[n & 3];
}

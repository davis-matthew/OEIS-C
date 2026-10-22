/*
 * A002940  Arrays of dumbbells.
 * https://oeis.org/A002940
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + A000045(n+1), a(-2) = a(-1) = a(0) =
 *            0
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 37, u64 n <= 82, u128 n <= 173,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t p3 = (seq_t)0; /* a(i-3) */
    seq_t p2 = (seq_t)0; /* a(i-2) */
    seq_t p1 = (seq_t)0; /* a(i-1) */
    seq_t fa = (seq_t)1; /* F(i)   */
    seq_t fb = (seq_t)1; /* F(i+1) */
    idx_t i;
    for (i = 1; i <= n; i++) {
        seq_t cur = (seq_t)2 * p1 + fb - p3;
        seq_t fn = fa + fb;
        p3 = p2;
        p2 = p1;
        p1 = cur;
        fa = fb;
        fb = fn;
    }
    return p1;
}

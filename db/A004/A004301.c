/*
 * A004301  Second-order Eulerian numbers <<n,2>>.
 * https://oeis.org/A004301
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*3^n - (12+8*n)*2^n + (3+6*n+4*n^2))/2, computed
 *            without a final division as a(n) = 3*h(n+1) - (6+4*n)*2^n + 3*n
 *            + 2*n^2, where h(m) = (3^m+1)/2, h(0) = 1, h(m) = 3*h(m-1) - 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t h = 1;      /* h(0) = (3^0+1)/2 */
    seq_t p2 = 1;     /* 2^0 */
    idx_t i;
    for (i = 0; i < n + 1; i++) {
        h = 3 * h - 1;
    }
    for (i = 0; i < n; i++) {
        p2 *= 2;
    }
    seq_t ns = (seq_t)n;
    seq_t pos = 3 * h + 3 * ns + 2 * ns * ns;
    seq_t neg = (6 + 4 * ns) * p2;
    return pos - neg;
}

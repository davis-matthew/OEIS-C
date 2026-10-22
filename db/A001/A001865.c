/*
 * A001865  Number of connected functions on n labeled nodes.
 * https://oeis.org/A001865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k=1..n} n!*n^(n-k-1)/(n-k)! = Sum_{k=1..n}
 *            (n-1)!/(n-k)! * n^(n-k), computed with Horner's method: g = 1;
 *            for j = 1..n-1: g = n^j + j*g; a(n) = g
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t g = 1;
    seq_t p = 1;
    for (idx_t j = 1; j < n; j++) {
        p *= (seq_t)n;
        g = p + (seq_t)j * g;
    }
    return g;
}

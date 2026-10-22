/*
 * A006231  a(n) = Sum_{k=2..n} n(n-1)...(n-k+1)/k.
 * https://oeis.org/A006231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n+1) = a(n) + A000522(n) - 1, a(1) = 0, where A000522(m) =
 *            m*A000522(m-1) + 1, A000522(0) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t res = 0;   /* a(1) */
    seq_t b = 1;     /* A000522(0) */
    idx_t m;
    for (m = 1; m < n; m++) {
        b = b * (seq_t)m + 1;   /* A000522(m) */
        res += b - 1;           /* a(m+1) */
    }
    return res;
}

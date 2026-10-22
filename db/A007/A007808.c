/*
 * A007808  Number of directed column-convex polyominoes of height n:
 *          a(k+1)=(k+1)*a(k)+(a(1)+...+a(k)).
 * https://oeis.org/A007808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(k+1) = (k+1)*a(k) + (a(1)+...+a(k)), a(0) = 1, a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    seq_t cur = 1; /* a(1) */
    seq_t sum = 1; /* a(1)+...+a(k) */
    for (idx_t k = 1; k < n; k++) {
        seq_t next = (seq_t)(k + 1) * cur + sum;
        cur = next;
        sum += cur;
    }
    return cur;
}

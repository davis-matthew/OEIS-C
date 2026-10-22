/*
 * A001697  a(n+1) = a(n)*(a(0) + ... + a(n)), a(0) = 1.
 * https://oeis.org/A001697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n+1) = a(n)*(a(0) + ... + a(n)), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 6, u64 n <= 7, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t cur = 1;
    seq_t sum = 1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        cur = cur * sum;
        sum += cur;
    }
    return cur;
}

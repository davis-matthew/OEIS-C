/*
 * A033590  a(n) = (2*n-1)*(3*n-1)*(4*n-1)*(5*n-1).
 * https://oeis.org/A033590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n-1)*(3*n-1)*(4*n-1)*(5*n-1)
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 77, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 1;
    seq_t x = (seq_t)n;
    seq_t p1 = (2 * x) - 1;
    seq_t p2 = (3 * x) - 1;
    seq_t p3 = (4 * x) - 1;
    seq_t p4 = (5 * x) - 1;
    return ((p1 * p2) * (p3 * p4));
}

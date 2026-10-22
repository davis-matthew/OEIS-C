/*
 * A004218  a(n) = log_10(n) rounded up.
 * https://oeis.org/A004218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(log_10(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    idx_t m = n - 1;
    seq_t count = 0;
    while (m > 0) {
        count++;
        m /= 10;
    }
    return count;
}

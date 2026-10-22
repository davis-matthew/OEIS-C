/*
 * A004216  a(n) = floor(log_10(n)).
 * https://oeis.org/A004216
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(log_10(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 10) return 0;
    seq_t count = 0;
    while (n >= 10) {
        n /= 10;
        count++;
    }
    return count;
}

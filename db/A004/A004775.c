/*
 * A004775  Numbers k such that the binary expansion of k does not end in 011.
 * https://oeis.org/A004775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n - 1 for n < 12; a(n) = n + floor((n-12)/7) for n >= 12
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 12) {
        return (seq_t)(n - 1);
    } else {
        return (seq_t)(n + (n - 12) / 7);
    }
}

/*
 * A004151  Omit trailing zeros from n.
 * https://oeis.org/A004151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = a(n/10) if n mod 10 = 0, otherwise n
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    while (n % 10 == 0) {
        n = n / 10;
    }
    return (seq_t)n;
}

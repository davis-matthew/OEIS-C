/*
 * A004275  1 together with nonnegative even numbers.
 * https://oeis.org/A004275
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0, a(1) = 1, a(n) = 2n - 2 for n >= 2
 * Range:     u8 n <= 128, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return 2 * (seq_t)n - 2;
}

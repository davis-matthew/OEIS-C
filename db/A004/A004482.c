/*
 * A004482  Tersum n + 1 (answer recorded in base 10).
 * https://oeis.org/A004482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 3*floor(n/3) + ((n+1) mod 3)
 * Range:     u8 n <= 254, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)(3 * (n / 3)) + (seq_t)((n + 1) % 3);
}

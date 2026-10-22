/*
 * A004776  Numbers not congruent to 5 (mod 8).
 * https://oeis.org/A004776
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor((n-6)/7)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n <= 5) {
        return (seq_t)(n - 1);
    } else {
        return (seq_t)n + (seq_t)((n - 6) / 7);
    }
}

/*
 * A047461  Numbers that are congruent to {1, 4} mod 8.
 * https://oeis.org/A047461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/2) + ((n mod 2) ? 1 : 4)
 * Range:     u8 n <= 64, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return 0;                 /* safety: offset is 1 */
    idx_t k = (n - 1) / 2;                 /* floor((n-1)/2) */
    seq_t val = (seq_t)8 * k + ((n & 1) ? 1 : 4);
    return val;
}

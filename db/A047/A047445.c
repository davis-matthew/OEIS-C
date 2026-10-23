/*
 * A047445  Numbers that are congruent to {1, 3, 5, 6} mod 8.
 * https://oeis.org/A047445
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/4) + {1,3,5,6}[(n-1)%4]
 * Range:     u8 n <= 63, u16 n <= 63, u32 n <= 63, u64 n <= 63, u128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n must be >= 1 */
    idx_t q = (n - 1) / 4;
    idx_t r = (n - 1) % 4;
    static const seq_t map[4] = {1, 3, 5, 6};
    seq_t result = 8 * (seq_t)q + map[r];
    return result;
}

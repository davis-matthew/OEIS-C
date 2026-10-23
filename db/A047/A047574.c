/*
 * A047574  Numbers that are congruent to {5, 6, 7} mod 8.
 * https://oeis.org/A047574
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*floor((n-1)/3) + ((n-1) mod 3) + 5
 * Range:     u8 n <= 96, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* n is unsigned, guaranteed n >= 1 */
    seq_t k = (seq_t)(n - 1);          /* k = n-1 */
    seq_t q = k / 3;                   /* q = floor((n-1)/3) */
    seq_t r = k % 3;                   /* r = (n-1) mod 3 */
    return q * 8 + r + 5;
}

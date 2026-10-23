/*
 * A010883  Simple periodic sequence: repeat [1, 2, 3, 4].
 * https://oeis.org/A010883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + (n mod 4)
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* compute 1 + (n modulo 4) */
    return (seq_t)(n % 4) + 1;
}

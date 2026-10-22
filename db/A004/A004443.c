/*
 * A004443  Nimsum n + 2.
 * https://oeis.org/A004443
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n XOR 2
 * Range:     u8 n <= 255, u16 n <= 9596, u32 n <= 9596, u64 n <= 9596,
 *            u128 n <= 9596, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)n ^ 2;
}

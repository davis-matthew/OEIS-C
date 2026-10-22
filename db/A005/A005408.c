/*
 * A005408  The odd numbers: a(n) = 2*n + 1.
 * https://oeis.org/A005408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, core, nice, easy
 * Formula:   a(n) = 2*n + 1
 * Range:     u8 n <= 127, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)n * 2 + 1;
}

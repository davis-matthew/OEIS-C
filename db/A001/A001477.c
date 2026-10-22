/*
 * A001477  The nonnegative integers.
 * https://oeis.org/A001477
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, nonn, easy, mult, tabl
 * Formula:   a(n) = n
 * Range:     u8 n <= 255, u16 n <= 9161, u32 n <= 9161, u64 n <= 9161,
 *            u128 n <= 9161, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (seq_t)n;
}

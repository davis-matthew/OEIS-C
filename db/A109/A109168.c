/*
 * A109168  Continued fraction expansion of the constant x (A109169) such that
 *          the continued fraction of 2*x yields the continued fraction of x
 *          interleaved with the positive even numbers.
 * https://oeis.org/A109168
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, nonn
 * Formula:   a(n) = n - ((n & (n - 1)) >> 1)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    seq_t y = x & (x - 1);
    return x - (y >> 1);
}

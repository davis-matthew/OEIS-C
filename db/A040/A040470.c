/*
 * A040470  Continued fraction for sqrt(493).
 * https://oeis.org/A040470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less
 * Formula:   a(0) = 22; for n > 0, a(n) = pattern[(n-1) % 9] where pattern =
 *            [4,1,10,3,3,10,1,4,44] (continued fraction of sqrt(493))
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)22;
    /* Periodic part of the continued fraction */
    static const seq_t pattern[9] = {4,1,10,3,3,10,1,4,44};
    const idx_t pos = (n - 1) % 9;   /* n is unsigned, modulo is safe */
    return pattern[pos];
}

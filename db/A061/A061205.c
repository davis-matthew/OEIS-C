/*
 * A061205  a(n) = n times R(n) where R(n) (A004086) is the digit reversal of
 *          n.
 * https://oeis.org/A061205
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n * rev(n) where rev(n) is the decimal digit reversal of
 *            n
 * Range:     u8 n <= 12, u16 n <= 106, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Compute rev(n) in arbitrary precision (only uses seq_t operations). */
    seq_t rev = 0;
    idx_t t = n;
    while (t > 0) {
        rev = rev * 10 + (seq_t)(t % 10);
        t /= 10;
    }
    return (seq_t)n * rev;
}

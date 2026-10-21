/*
 * A217557  The difference between the reversal of an 8-bit integer and the
 *          original integer.
 * https://oeis.org/A217557
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, fini, full
 * Formula:   a(n) = A160638(n) - n
 * Range:     i8 n <= 2, i16 n <= 256, i32 n <= 256, i64 n <= 256,
 *            i128 n <= 256, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute the bit reversal of the lowest 8 bits of n */
    uint8_t v = (uint8_t)(n & 0xFFu);
    uint8_t rev = 0;
    for (int i = 0; i < 8; ++i) {
        rev = (rev << 1) | (v & 0x01u);
        v >>= 1;
    }
    return (seq_t)rev - (seq_t)n;
}

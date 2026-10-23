/*
 * A160638  Bit-reversed 8-bit binary numbers.
 * https://oeis.org/A160638
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, easy, fini, full, nonn, nice
 * Formula:   a(n) = floor(A030101(n+256)/2)
 * Range:     u8 n <= 255, u16 n <= 255, u32 n <= 255, u64 n <= 255,
 *            u128 n <= 255, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes bit-reversed 8-bit value of n. */
seq_t a(idx_t n)
{
    unsigned char v   = (unsigned char)(n & 0xFF);
    unsigned char rev = 0;
    unsigned char i;
    for (i = 0; i < 8; ++i) {
        rev = (rev << 1) | (v & 1);
        v >>= 1;
    }
    return (seq_t)rev;
}

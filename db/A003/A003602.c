/*
 * A003602  Kimberling's paraphrase of the binary number system: if n =
 *          (2k-1)*2^m then a(n) = k.
 * https://oeis.org/A003602
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, hear
 * Formula:   a(n) = (A000265(n) + 1)/2, where A000265(n) is the odd part of n
 * Range:     u8 n <= 510, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    while ((n & 1) == 0) {
        n >>= 1;
    }
    return (seq_t)((n + 1) >> 1);
}

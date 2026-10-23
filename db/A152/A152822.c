/*
 * A152822  Periodic sequence [1,1,0,1] of length 4.
 * https://oeis.org/A152822
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, mult
 * Formula:   a(n) = (n % 4 == 2) ? 0 : 1
 * Range:     u8 n <= 8815, u16 n <= 8815, u32 n <= 8815, u64 n <= 8815,
 *            u128 n <= 8815, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    return (n % 4 == 2) ? 0 : 1;
}

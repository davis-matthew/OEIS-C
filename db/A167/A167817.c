/*
 * A167817  Period 4: repeat [1, 3, 3, 3].
 * https://oeis.org/A167817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  frac, nonn, easy
 * Formula:   a(n) = 3 - 2 * (n mod 4 == 0)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    /* Return 1 when n is a multiple of 4, otherwise 3.   */
    return ((n & 3) == 0) ? (seq_t)1 : (seq_t)3;
}

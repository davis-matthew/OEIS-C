/*
 * A006519  Highest power of 2 dividing n.
 * https://oeis.org/A006519
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice, mult, hear, changed
 * Formula:   a(n) = n AND -n (bitwise AND with two's complement)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t x = (seq_t)n;
    return x & (-x);
}

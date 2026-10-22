/*
 * A007814  Exponent of highest power of 2 dividing n, a.k.a. the binary carry
 *          sequence, the ruler sequence, or the 2-adic valuation of n.
 * https://oeis.org/A007814
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 0 if n is odd, otherwise 1 + a(n/2)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t count = 0;
    while ((n & 1) == 0) {
        count++;
        n >>= 1;
    }
    return count;
}

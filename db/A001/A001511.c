/*
 * A001511  The ruler function: exponent of the highest power of 2 dividing
 *          2n. Equivalently, the 2-adic valuation of 2n.
 * https://oeis.org/A001511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, nice, easy, core, hear
 * Formula:   a(n) = A007814(2*n), the exponent of the highest power of 2
 *            dividing 2n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    seq_t result = 1;
    while ((n & 1) == 0) {
        result++;
        n >>= 1;
    }
    return result;
}

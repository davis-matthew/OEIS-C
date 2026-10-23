/*
 * A102377  Gould's sequence A001316 in binary.
 * https://oeis.org/A102377
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, base, nonn
 * Formula:   a(n) = 10^{popcount(n)} (A000120(n) is the number of 1-bits in
 *            n)
 * Range:     u8 n <= 6, u16 n <= 30, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count the number of set bits in n (popcount). */
    int bits = 0;
    idx_t m = n;
    while (m) {
        bits++;         /* increment for the lowest set bit */
        m &= m - 1;     /* clear the lowest set bit */
    }

    /* Compute 10^bits in a type-correct way. */
    seq_t result = 1;
    for (int i = 0; i < bits; ++i)
        result *= 10;

    return result;
}

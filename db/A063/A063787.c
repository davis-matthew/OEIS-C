/*
 * A063787  a(2^k) = k + 1 and a(2^k + i) = 1 + a(i) for k >= 0 and 0 < i <
 *          2^k.
 * https://oeis.org/A063787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = popcount(n-1) + 1 (Hamming weight of the odd number 2k+1
 *            where n = 2k+1)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute popcount of n-1 */
    idx_t x = n - 1;
    unsigned int pop = 0;
    while (x) {
        x &= x - 1;  /* clear lowest set bit */
        ++pop;
    }
    return (seq_t)pop + 1;
}

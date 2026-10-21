/*
 * A080791  Number of nonleading 0's in binary expansion of n.
 * https://oeis.org/A080791
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 0; for n > 0, a(n) = (number of bits of n) – popcount(n)
 *            i.e. a(n) = floor(log2(n)) + 1 – popcount(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    idx_t temp, len = 0, pop = 0;

    /* count bit length (number of bits) */
    temp = n;
    while (temp) {
        ++len;
        temp >>= 1;
    }

    /* count 1-bits (popcount) */
    temp = n;
    while (temp) {
        temp &= temp - 1;
        ++pop;
    }

    return (seq_t)(len - pop);
}

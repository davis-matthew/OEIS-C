/*
 * A033047  Sums of distinct powers of 11.
 * https://oeis.org/A033047
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Sum_{i=0..m} d(i)*11^i, where d(i) are the binary digits
 *            of n
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 1023, u64 n <= 1023,
 *            u128 n <= 1023, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 0;   /* accumulated sum of 11^i for every set bit */
    seq_t power  = 1;   /* 11^i for the current bit position i */

    /* Iterate over the binary representation of n, from LSB to MSB */
    while (n != 0) {
        if (n & 1ULL)       /* bit is 1: include this power of 11 */
            result += power;

        power *= 11;        /* advance to the next power of 11 */
        n >>= 1ULL;         /* move to the next bit */
    }

    return result;
}

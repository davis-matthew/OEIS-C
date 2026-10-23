/*
 * A152723  In binary, count of least frequent bit of n.
 * https://oeis.org/A152723
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = min(popcount(n), floor(log2(n)) + 1 - popcount(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Count bits and ones in the binary representation of n. */
    idx_t m = n;
    unsigned ones = 0;
    unsigned len  = 0;
    while (m) {
        if (m & 1ULL) ones++;
        m >>= 1;
        len++;
    }
    unsigned zeros = len - ones;
    return (seq_t)(ones < zeros ? ones : zeros);
}

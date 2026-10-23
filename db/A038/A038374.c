/*
 * A038374  Length of longest contiguous block of 1's in binary expansion of
 *          n.
 * https://oeis.org/A038374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, tabf
 * Formula:   a(n) = length of longest contiguous block of 1's in the binary
 *            expansion of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is 1-based; n == 0 should never occur. */
    idx_t x = n;
    seq_t longest = 0;
    seq_t current = 0;

    while (x) {
        if (x & 1) {
            ++current;
            if (current > longest) longest = current;
        } else {
            current = 0;
        }
        x >>= 1;
    }

    return (seq_t)longest;
}

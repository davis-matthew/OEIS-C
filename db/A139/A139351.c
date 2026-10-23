/*
 * A139351  Let the binary expansion of n be n = Sum_{k} 2^{r_k}, let e(n) be
 *          the number of r_k's that are even, o(n) the number that are odd;
 *          sequence gives e(n).
 * https://oeis.org/A139351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = Σ_{k≥0} ((n >> (2k)) & 1) counts the number of 1-bits in
 *            the binary expansion of n whose position is even (positions
 *            counted from 0)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;
    while (n) {
        if (n & 1ULL)  /* bit at an even position (LSB is pos 0) */
            ++count;
        n >>= 2;       /* skip the next (odd) bit */
    }
    return count;
}

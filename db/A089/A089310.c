/*
 * A089310  Write n in binary; a(n) = number of 1's in second block of 1's
 *          from right.
 * https://oeis.org/A089310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of 1's in the second block of consecutive 1's *
 *            when writing n in binary and reading from the least‑significant
 *            bit * side. If n has fewer than two blocks of 1's, a(n) = 0
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t t = n;
    /* skip trailing zeros (bits below the first 1) */
    while (t && !(t & 1)) t >>= 1;
    if (!t) return 0;            /* no 1's at all */

    /* skip the first block of 1's */
    while (t && (t & 1)) t >>= 1;
    /* skip the zero block after the first block of 1's */
    while (t && !(t & 1)) t >>= 1;

    /* count the second block of 1's */
    idx_t count = 0;
    while (t && (t & 1)) {
        count++;
        t >>= 1;
    }
    return (seq_t)count;
}

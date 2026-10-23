/*
 * A014082  Number of occurrences of '111' in binary expansion of n.
 * https://oeis.org/A014082
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = number of times the pattern '111' occurs in the binary
 *            representation of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t count = 0;          /* result to return */
    idx_t x = n;              /* working copy of the input */

    /* Inspect all consecutive 3-bit windows. */
    while (x >= 7)            /* at least 3 bits remain */
    {
        if ((x & 7) == 7)     /* lowest 3 bits equal 111? */
            count++;          /* increment the occurrence counter */

        x >>= 1;              /* slide window one bit to the right */
    }

    return count;
}

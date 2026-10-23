/*
 * A036988  Has simplest possible tree complexity of all transcendental
 *          sequences.
 * https://oeis.org/A036988
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n)=1 iff, when scanning the binary expansion of n from
 *            least‑significant bit to most, the number of 1’s never exceeds
 *            the number of 0’s
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t x = n;          /* local copy so the original argument is not modified */
    int zeros = 0;       /* count of zeros seen so far (in the reversed binary) */
    int ones  = 0;       /* count of ones  seen so far */

    while (x)
    {
        if ((x & 1ULL) == 0)          /* the current LSB is 0 */
            ++zeros;
        else                          /* the current LSB is 1 */
        {
            ++ones;
            if (ones > zeros)        /* invalid if ones ever exceed zeros */
                return 0;
        }
        x >>= 1;                      /* move to the next bit */
    }

    return 1;                         /* property holds for all bits */
}

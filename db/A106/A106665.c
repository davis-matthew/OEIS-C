/*
 * A106665  Alternate paper-folding (or alternate dragon curve) sequence.
 * https://oeis.org/A106665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(4n) = 1, a(4n+2) = 0, a(2n+1) = 1 - a(n)
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Even n */
    if ((n & 1) == 0) {
        /* n mod 4 == 2 -> a(n) = 0 */
        if (n & 2) /* check second least significant bit */
            return (seq_t)0;
        /* n divisible by 4 or n == 0 -> a(n) = 1 */
        return (seq_t)1;
    }
    /* Odd n: a(n) = 1 - a(n/2) */
    return (seq_t)(1 - a(n >> 1));
}

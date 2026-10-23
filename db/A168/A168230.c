/*
 * A168230  a(n) = n + 2 - a(n-1) for n>1; a(1) = 0.
 * https://oeis.org/A168230
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n even) ? n/2 + 3 : (n-1)/2
 * Range:     u8 n <= 306, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is at least 1. */
    if (n & 1U) {
        /* n is odd: a(n) = (n-1)/2 */
        return (seq_t)(n - 1U) / 2U;
    } else {
        /* n is even: a(n) = n/2 + 3 */
        return ((seq_t)n / 2U) + (seq_t)3U;
    }
}

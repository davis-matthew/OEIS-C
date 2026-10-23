/*
 * A013710  a(n) = 5^(2*n + 1).
 * https://oeis.org/A013710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^(2*n + 1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    idx_t exp = 2*n + 1;   /* exponent: 2*n+1 */
    while (exp--)
        result *= 5;       /* multiply by 5 iteratively */
    return result;
}

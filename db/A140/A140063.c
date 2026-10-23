/*
 * A140063  Binomial transform of [1, 3, 7, 0, 0, 0, ...].
 * https://oeis.org/A140063
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*n^2/2 - n/2 + 1
 * Range:     u8 n <= 8, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    /* (7 * n * n - n + 2) / 2 */
    return (( (seq_t)7 * nn * nn - nn + 2 ) / 2);
}

/*
 * A022276  a(n) = n*(19*n - 1)/2.
 * https://oeis.org/A022276
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(19*n - 1)/2
 * Range:     u8 n <= 5, u16 n <= 83, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;            /* convert index to seq_t */
    seq_t prod = (seq_t)19 * nn - 1; /* 19*n - 1 */
    return nn * prod / 2;           /* n*(19*n - 1)/2 */
}

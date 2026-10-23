/*
 * A157448  a(n) = 2048*n^2 - 128*n + 1.
 * https://oeis.org/A157448
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2048*n^2 - 128*n + 1
 * Range:     u8 none, u16 n <= 5, u32 n <= 1448, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the index to the sequence type first */
    seq_t nn = (seq_t)n;
    seq_t tmp = nn * nn;               /* n^2 */
    return (2048 * tmp) - (128 * nn) + 1;
}

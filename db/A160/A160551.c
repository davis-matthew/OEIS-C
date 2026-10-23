/*
 * A160551  Number of unordered ways of making change for n dollars using
 *          coins of denominations 1, 5, 10, and 25.
 * https://oeis.org/A160551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3 + 53*n + 270*n^2 + 400*n^3)/3
 * Range:     u8 n <= 0, u16 n <= 5, u32 n <= 229, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t nn2 = nn * nn;
    seq_t nn3 = nn2 * nn;
    seq_t sum = 3 + (seq_t)53 * nn + (seq_t)270 * nn2 + (seq_t)400 * nn3;
    return sum / 3;
}

/*
 * A208536  Number of 5-bead necklaces of n colors not allowing reversal, with
 *          no adjacent beads having the same color.
 * https://oeis.org/A208536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((n-1)^5 - (n-1))/5
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 91, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* safety for unexpected calls */
    seq_t x = (seq_t)(n - 1);
    seq_t x2 = x * x;
    seq_t x3 = x2 * x;
    seq_t x4 = x3 * x;
    seq_t x5 = x4 * x;
    return (x5 - x) / (seq_t)5;
}

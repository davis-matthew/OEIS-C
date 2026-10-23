/*
 * A158627  a(n) = 484*n^2 - 22.
 * https://oeis.org/A158627
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 484*n^2 - 22
 * Range:     u8 none, u16 n <= 11, u32 n <= 2978, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn = (seq_t)n;          /* cast index to seq_t early */
    return tn * tn * 484 - 22;    /* (484*n^2) - 22 */
}

/*
 * A235088  a(n)*Pi is the total length of irregular spiral (center points: 1,
 *          2, 3, 4) after n rotations.
 * https://oeis.org/A235088
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 + n - 4*floor(n/2)
 * Range:     u8 n <= 11, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert the index to the sequence's integer type */
    seq_t nn   = (seq_t)n;
    /* floor(n/2) can be obtained by shifting right by one bit */
    seq_t half = nn >> 1;          /* same as floor(nn/2) */
    /* Compute 2*n^2 + n - 4*floor(n/2) */
    seq_t res  = 2 * nn * nn + nn - 4 * half;
    return res;
}

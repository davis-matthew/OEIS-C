/*
 * A187326  a(n) = floor(n/4) + floor(n/2) + floor(3n/4).
 * https://oeis.org/A187326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/4) + floor(n/2) + floor(3*n/4)
 * Range:     u8 n <= 171, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use the wider idx_t type for intermediate arithmetic to avoid overflow
       in very small seq_t types, then cast the results back to seq_t. */
    idx_t nn = n;
    seq_t part1 = (seq_t)(nn / 4);               /* floor(n/4)  */
    seq_t part2 = (seq_t)(nn / 2);               /* floor(n/2)  */
    seq_t part3 = (seq_t)((nn * 3U) / 4);        /* floor(3*n/4) */
    return part1 + part2 + part3;
}

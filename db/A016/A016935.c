/*
 * A016935  a(n) = (6*n + 2)^3.
 * https://oeis.org/A016935
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 2)^3
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 270, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)6 * n + 2;   /* compute (6*n + 2) */
    return (t * t) * t;           /* cube it */
}

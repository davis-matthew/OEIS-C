/*
 * A017320  a(n) = (10*n + 4)^4.
 * https://oeis.org/A017320
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n+4)^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 25, u64 n <= 6553, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * 10 + 4;      /* compute (10*n + 4) */
    seq_t sq = t * t;    /* compute (10*n + 4)^2 */
    return sq * sq;      /* square again for ^4 */
}

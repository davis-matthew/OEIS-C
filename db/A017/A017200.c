/*
 * A017200  a(n) = (9*n+3)^4.
 * https://oeis.org/A017200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n+3)^4
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 28, u64 n <= 7281,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute t = 9*n + 3 as seq_t to avoid truncation issues. */
    seq_t t = ((seq_t)9 * n) + 3;

    /* Compute t squared and then t to the fourth power. */
    seq_t sq = t * t;   /* t^2 */
    return sq * sq;     /* (t^2)^2 = t^4 */
}

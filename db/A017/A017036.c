/*
 * A017036  a(n) = (7*n+4)^8.
 * https://oeis.org/A017036
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 4)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 35, u128 n <= 9361,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = ((seq_t)7) * n + 4;
    seq_t r = t;   /* t^1 */
    r *= t;        /* t^2 */
    r *= r;        /* t^4 */
    r *= r;        /* t^8 */
    return r;
}

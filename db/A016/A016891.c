/*
 * A016891  a(n) = (5*n+3)^7.
 * https://oeis.org/A016891
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5*n + 3)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)5 * n + 3;   /* compute 5*n + 3 */
    seq_t r = t * t;              /* t^2 */
    r *= t;                       /* t^3 */
    r *= t;                       /* t^4 */
    r *= t;                       /* t^5 */
    r *= t;                       /* t^6 */
    r *= t;                       /* t^7 */
    return r;
}

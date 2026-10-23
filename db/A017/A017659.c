/*
 * A017659  a(n) = (12*n + 11)^7.
 * https://oeis.org/A017659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 11)^7
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 46, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)12 * (seq_t)n + (seq_t)11; /* t = 12*n + 11 */
    seq_t r = t * t;      /* t^2 */
    r = r * t;            /* t^3 */
    r = r * t;            /* t^4 */
    r = r * t;            /* t^5 */
    r = r * t;            /* t^6 */
    r = r * t;            /* t^7 */
    return r;
}

/*
 * A017203  a(n) = (9*n + 3)^7.
 * https://oeis.org/A017203
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n + 3)^7
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 62, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * (seq_t)9 + (seq_t)3;   /* t = 9*n + 3 */
    seq_t r = t * t;              /* r = t^2 */
    r = r * t;                    /* r = t^3 */
    r = r * t;                    /* r = t^4 */
    r = r * t;                    /* r = t^5 */
    r = r * t;                    /* r = t^6 */
    r = r * t;                    /* r = t^7 */
    return r;
}

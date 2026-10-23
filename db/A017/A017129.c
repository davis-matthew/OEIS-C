/*
 * A017129  a(n) = (8*n + 5)^5.
 * https://oeis.org/A017129
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 5)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 9, u64 n <= 890, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 8*n + 5 */
    seq_t x = (seq_t)n;
    x = x * (seq_t)8 + (seq_t)5;

    /* compute x^5 */
    seq_t r = x;
    r = r * x; /* x^2 */
    r = r * x; /* x^3 */
    r = r * x; /* x^4 */
    r = r * x; /* x^5 */

    return r;
}

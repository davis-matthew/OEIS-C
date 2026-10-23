/*
 * A017151  a(n) = (8*n + 7)^3.
 * https://oeis.org/A017151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 7)^3
 * Range:     u8 none, u16 n <= 4, u32 n <= 202, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    x = x * 8 + 7;   /* x = 8*n + 7 */
    seq_t t = x * x; /* t = (8*n + 7)^2 */
    return t * x;    /* x^3 */
}

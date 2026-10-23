/*
 * A017586  a(n) = (12*n + 5)^6.
 * https://oeis.org/A017586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 5)^6
 * Range:     u8 none, u16 n <= 0, u32 n <= 2, u64 n <= 135, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)12 * (seq_t)n + (seq_t)5;
    seq_t t = base * base;   /* base^2 */
    t = t * base;            /* base^3 */
    return t * t;            /* base^6 */
}

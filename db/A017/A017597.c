/*
 * A017597  a(n) = (12*n + 6)^5.
 * https://oeis.org/A017597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 6)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 6, u64 n <= 593, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;
    seq_t base = (seq_t)12 * tn + 6;

    seq_t t2 = base * base;   /* base^2 */
    seq_t t3 = t2 * base;     /* base^3 */
    seq_t t4 = t3 * base;     /* base^4 */
    seq_t t5 = t4 * base;     /* base^5 */

    return t5;
}

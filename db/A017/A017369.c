/*
 * A017369  a(n) = (10*n + 8)^5.
 * https://oeis.org/A017369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 8)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 712, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t before any arithmetic */
    seq_t base = (seq_t)10 * (seq_t)n + (seq_t)8;

    /* Compute base^5 using only + - * / % etc. */
    seq_t b2 = base * base;           /* base^2 */
    seq_t b3 = b2 * base;              /* base^3 */
    seq_t b4 = b3 * base;              /* base^4 */
    seq_t b5 = b4 * base;              /* base^5 */

    return b5;
}

/*
 * A017646  a(n) = (12*n + 10)^6.
 * https://oeis.org/A017646
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 10)^6
 * Range:     u8 none, u16 none, u32 n <= 2, u64 n <= 134, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 12*n + 10 */
    seq_t base = (seq_t)n * (seq_t)12 + (seq_t)10;

    /* base^2 */
    seq_t x = base * base;
    /* base^4 */
    seq_t y = x * x;
    /* base^6 = base^2 * base^4 */
    seq_t result = x * y;

    return result;
}

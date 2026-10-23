/*
 * A017048  a(n) = (7*n + 5)^8.
 * https://oeis.org/A017048
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7*n + 5)^8
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 35, u128 n <= 9361,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert idx_t to seq_t and compute the base */
    seq_t base = (seq_t)n * 7 + 5;
    /* Compute base^8 by squaring thrice */
    seq_t res = base * base;   /* base^2 */
    res = res * res;           /* base^4 */
    res = res * res;           /* base^8 */
    return res;
}

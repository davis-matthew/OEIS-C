/*
 * A017408  a(n) = (11*n + 1)^8.
 * https://oeis.org/A017408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 1)^8
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 23, u128 n <= 5957,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t tn   = (seq_t)n;
    seq_t base = (seq_t)11 * tn + (seq_t)1;

    /* compute base^8 using exponentiation by squaring */
    seq_t res = base * base;  /* base^2 */
    res = res * res;          /* base^4 */
    res = res * res;          /* base^8 */
    return res;
}

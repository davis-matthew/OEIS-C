/*
 * A017172  a(n) = (9*n)^12.
 * https://oeis.org/A017172
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*n)^12
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 0, u64 n <= 4, u128 n <= 180,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x    = (seq_t)n * 9;   /* 9 * n  */
    seq_t pow2 = x * x;          /* x^2    */
    seq_t pow4 = pow2 * pow2;    /* x^4    */
    seq_t pow8 = pow4 * pow4;    /* x^8    */
    seq_t result = pow8 * pow4;  /* x^12   */
    return result;
}

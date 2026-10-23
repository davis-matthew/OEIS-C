/*
 * A017648  a(n) = (12*n + 10)^8.
 * https://oeis.org/A017648
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n+10)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 20, u128 n <= 5460,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute base = 12*n + 10 */
    seq_t base = (seq_t)12 * (seq_t)n + (seq_t)10;

    /* power by repeated squaring: base^2, base^4, base^8 */
    seq_t b2 = base * base;      /* base^2   */
    seq_t b4 = b2 * b2;          /* base^4   */
    seq_t b8 = b4 * b4;          /* base^8   */

    return b8;
}

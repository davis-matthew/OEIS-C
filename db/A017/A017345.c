/*
 * A017345  a(n) = (10*n + 6)^5.
 * https://oeis.org/A017345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n + 6)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 7, u64 n <= 712, u128 n <= 10000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* convert n to seq_t once */
    seq_t nn = (seq_t)n;

    /* base = 10*n + 6 */
    seq_t base = (seq_t)10 * nn + (seq_t)6;

    /* compute base^5 */
    seq_t res = base;
    res *= base;   /* base^2   */
    res *= base;   /* base^3   */
    res *= base;   /* base^4   */
    res *= base;   /* base^5   */

    return res;
}

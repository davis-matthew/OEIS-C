/*
 * A017600  a(n) = (12*n + 6)^8.
 * https://oeis.org/A017600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (12*n + 6)^8
 * Range:     u8 none, u16 none, u32 n <= 0, u64 n <= 20, u128 n <= 5460,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t base = (seq_t)12 * (seq_t)n + 6;
    seq_t res = base * base;   /* (12n+6)^2   */
    res = res * res;           /* (12n+6)^4   */
    res = res * res;           /* (12n+6)^8   */
    return res;
}

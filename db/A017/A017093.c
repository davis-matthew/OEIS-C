/*
 * A017093  a(n) = (8*n + 2)^5.
 * https://oeis.org/A017093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (8*n + 2)^5
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 10, u64 n <= 891,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)8 * n + (seq_t)2;   /* compute 8*n + 2 */
    seq_t r = x * x;                    /* x^2   */
    r = r * x;                          /* x^3   */
    r = r * x;                          /* x^4   */
    r = r * x;                          /* x^5   */
    return r;
}

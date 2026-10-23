/*
 * A017513  a(n) = (11*n + 10)^5.
 * https://oeis.org/A017513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (11*n + 10)^5
 * Range:     u8 none, u16 none, u32 n <= 6, u64 n <= 647, u128 n <= 1000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = 11 * (seq_t)n + 10;
    seq_t t2 = t * t;   /* t^2   */
    seq_t t3 = t2 * t;  /* t^3   */
    seq_t t4 = t3 * t;  /* t^4   */
    seq_t t5 = t4 * t;  /* t^5   */
    return t5;
}

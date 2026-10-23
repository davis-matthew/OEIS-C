/*
 * A016961  a(n) = (6*n + 4)^5.
 * https://oeis.org/A016961
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^5
 * Range:     u8 none, u16 n <= 0, u32 n <= 13, u64 n <= 1187, u128 n <= 3000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute 6*n + 4 first */
    seq_t base = 6 * (seq_t)n + 4;

    /* base^2, base^4, and finally base^5 */
    seq_t sq      = base * base;      /* base^2   */
    seq_t sq_sq   = sq * sq;          /* base^4   */

    return sq_sq * base;              /* base^5   */
}

/*
 * A016745  a(n) = (2*n)^5.
 * https://oeis.org/A016745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (2*n)^5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 42, u64 n <= 3565,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;
    t = t * (seq_t)n;   /* n^2 */
    t = t * (seq_t)n;   /* n^3 */
    t = t * (seq_t)n;   /* n^4 */
    t = t * (seq_t)n;   /* n^5 */
    t = t * 32;         /* 2^5 * n^5 */
    return t;
}

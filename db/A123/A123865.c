/*
 * A123865  a(n) = n^4 - 1.
 * https://oeis.org/A123865
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 256, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t t = x * x;   /* n^2 */
    t = t * t;         /* n^4 */
    t = t - (seq_t)1;  /* n^4 - 1 */
    return t;
}

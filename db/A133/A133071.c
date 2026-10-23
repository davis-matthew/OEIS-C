/*
 * A133071  a(n) = n^5 - n^3 + n^2.
 * https://oeis.org/A133071
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^5 - n^3 + n^2
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 84, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t  = (seq_t)n;      /* n */
    seq_t n2 = t * t;         /* n^2 */
    seq_t n3 = n2 * t;        /* n^3 */
    seq_t n5 = n3 * n2;       /* n^5 = n^3 * n^2 */
    return n5 - n3 + n2;      /* n^5 - n^3 + n^2 */
}

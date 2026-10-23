/*
 * A016960  a(n) = (6*n + 4)^4.
 * https://oeis.org/A016960
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 4)^4
 * Range:     u8 none, u16 n <= 1, u32 n <= 41, u64 n <= 3000, u128 n <= 3000,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* t = 6*n + 4 */
    seq_t t = (seq_t)6 * n + 4;
    /* t2 = t * t */
    seq_t t2 = t * t;
    /* result = t2 * t2 = (t^2)^2 = t^4 */
    return t2 * t2;
}

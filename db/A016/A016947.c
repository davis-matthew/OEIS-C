/*
 * A016947  a(n) = (6*n + 3)^3.
 * https://oeis.org/A016947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n + 3)^3
 * Range:     u8 n <= 0, u16 n <= 6, u32 n <= 270, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n * 6 + 3;   /* t = 6*n + 3 */
    return t * t * t;             /* t^3 */
}

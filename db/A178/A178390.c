/*
 * A178390  a(n) = (n^2 + 1)^2 + 1.
 * https://oeis.org/A178390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 1)^2 + 1
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t t = (seq_t)n;          /* cast index to the value type */
    t = t * t + 1;               /* t = n^2 + 1   */
    t = t * t + 1;               /* t = (n^2 + 1)^2 + 1 */
    return t;
}

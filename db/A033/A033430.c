/*
 * A033430  a(n) = 4*n^3.
 * https://oeis.org/A033430
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^3
 * Range:     u8 n <= 3, u16 n <= 25, u32 n <= 750, u64 n <= 750,
 *            u128 n <= 750, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = n;
    x *= x;          /* x = n^2 */
    x *= n;          /* x = n^3 */
    x *= (seq_t)4;   /* x = 4*n^3 */
    return x;
}

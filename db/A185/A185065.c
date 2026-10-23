/*
 * A185065  a(n) = n*(n^3 + 2).
 * https://oeis.org/A185065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^3+2)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * n;  /* n^2 */
    nn *= n;                  /* n^3 */
    return (seq_t)n * (nn + 2);
}

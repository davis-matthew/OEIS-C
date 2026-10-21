/*
 * A214660  a(n) = 9*n^2 - 11*n + 3.
 * https://oeis.org/A214660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 9*n^2 - 11*n + 3
 * Range:     u8 n <= 5, u16 n <= 85, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t res = 9 * nn * nn;   /* compute 9*n^2 */
    res -= 11 * nn;            /* subtract 11*n */
    res += 3;                  /* add 3 */
    return res;
}

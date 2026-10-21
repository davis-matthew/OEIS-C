/*
 * A094200  a(n) = 16*n^4 + 32*n^3 + 36*n^2 + 20*n + 3.
 * https://oeis.org/A094200
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^4 + 32*n^3 + 36*n^2 + 20*n + 3
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    seq_t n2 = x * x;       /* n^2 */
    seq_t n3 = n2 * x;      /* n^3 */
    seq_t n4 = n3 * x;      /* n^4 */
    return 16 * n4 + 32 * n3 + 36 * n2 + 20 * x + 3;
}

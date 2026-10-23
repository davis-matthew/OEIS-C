/*
 * A145069  a(n) = n*(n^2 + 3*n + 5)/3.
 * https://oeis.org/A145069
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n^2 + 3*n + 5)/3
 * Range:     u8 n <= 8, u16 n <= 57, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t term = nn * nn + (seq_t)3 * nn + (seq_t)5; /* n^2 + 3*n + 5 */
    return nn * term / (seq_t)3;
}

/*
 * A158631  a(n) = 529*n^2 + 23.
 * https://oeis.org/A158631
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 529*n^2 + 23
 * Range:     u8 n <= 0, u16 n <= 11, u32 n <= 2849, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;          /* n^2 */
    seq_t res = (seq_t)529 * sq; /* 529*n^2 */
    return res + (seq_t)23;      /* +23 */
}

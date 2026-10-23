/*
 * A145678  a(n) = 441*n^2 - 21.
 * https://oeis.org/A145678
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 441*n^2 - 21
 * Range:     u8 none, u16 n <= 12, u32 n <= 3120, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* a(0) is outside the OEIS offset; return 0 to avoid unsigned underflow */
    if (n == 0) return (seq_t)0;
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;        /* n^2 */
    res *= (seq_t)441;           /* 441*n^2 */
    res -= (seq_t)21;            /* -21 */
    return res;
}

/*
 * A014409  Number of inequivalent ways (mod D_4) a pair of checkers can be
 *          placed on an n X n board.
 * https://oeis.org/A014409
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = (n^4 + 8 n^2 - 8 n - 1)/16 if n is odd n*(n^3 + 6 n -
 *            4)/16 if n is even
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 283, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* the sequence starts at n=1 */
    seq_t nS = (seq_t)n;
    seq_t n2 = nS * nS;                /* n^2 */
    seq_t n3 = n2 * nS;                /* n^3 */
    seq_t n4 = n3 * nS;                /* n^4 */

    if (nS & 1)                       /* odd n */
    {
        seq_t num = n4 + (seq_t)8 * n2 - (seq_t)8 * nS - (seq_t)1;
        return num / (seq_t)16;
    }
    else                               /* even n */
    {
        seq_t num = n3 + (seq_t)6 * nS - (seq_t)4;
        return (nS * num) / (seq_t)16;
    }
}

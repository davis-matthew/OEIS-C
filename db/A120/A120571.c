/*
 * A120571  2n^4+6n^2+4 = 2(n^2+1)(n^2+2).
 * https://oeis.org/A120571
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*(n^2+1)*(n^2+2) (offset 1)
 * Range:     u8 n <= 3, u16 n <= 13, u32 n <= 215, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* undefined for n=0, safe guard */

    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;               /* n^2 */
    return 2 * (sq + 1) * (sq + 2);    /* 2*(n^2+1)*(n^2+2) */
}

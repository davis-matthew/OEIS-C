/*
 * A226292  a(n) = (10*n^2+4*n+(1-(-1)^n))/8.
 * https://oeis.org/A226292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*n^2+4*n+(1-(-1)^n))/8
 * Range:     u8 n <= 4, u16 n <= 111, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t num = nn * nn;   /* n^2 */
    num *= 10;             /* 10*n^2 */
    num += nn * 4;         /* +4*n */
    if (n & 1)             /* add 2 when n is odd */
        num += 2;
    return num / 8;        /* division is exact */
}

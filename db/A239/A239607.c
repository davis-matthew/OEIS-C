/*
 * A239607  a(n) = (1-2*n^2)^2.
 * https://oeis.org/A239607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1-2*n^2)^2
 * Range:     u8 n <= 2, u16 n <= 11, u32 n <= 181, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t n2 = sn * sn;               /* n^2 */
    seq_t temp = (seq_t)2 * n2;       /* 2*n^2 */
    seq_t t = (seq_t)1 - temp;        /* 1 - 2*n^2 */
    return t * t;                     /* (1-2*n^2)^2 */
}

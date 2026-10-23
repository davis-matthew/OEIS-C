/*
 * A134161  a(n) = 373 + 1947*n + 3780*n^2 + 3234*n^3 + 1029*n^4.
 * https://oeis.org/A134161
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 373 + 1947*n + 3780*n^2 + 3234*n^3 + 1029*n^4
 * Range:     u8 none, u16 n <= 2, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;        /* cast the index to seq_t */
    seq_t x2 = x * x;           /* n^2   */
    seq_t x3 = x2 * x;          /* n^3   */
    seq_t x4 = x3 * x;          /* n^4   */

    seq_t res = 1029 * x4;      /* 1029*n^4   */
    res += 3234 * x3;           /* +3234*n^3   */
    res += 3780 * x2;           /* +3780*n^2   */
    res += 1947 * x;            /* +1947*n     */
    res += 373;                 /* +373        */

    return res;
}

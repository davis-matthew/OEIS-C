/*
 * A221576  Number of 0..n arrays of length 6 with each element differing from
 *          at least one neighbor by something other than 1.
 * https://oeis.org/A221576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=10, a(2)=141, a(3)=1210; for n>3, a(n) = n^6 + 2*n^5 -
 *            5*n^4 + 24*n^3 - 41*n^2 + 50*n - 31
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 40, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)141;
    if (n == 3) return (seq_t)1210;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;           /* n^2 */
    seq_t n3 = n2 * nn;           /* n^3 */
    seq_t n4 = n3 * nn;           /* n^4 */
    seq_t n5 = n4 * nn;           /* n^5 */
    seq_t n6 = n5 * nn;           /* n^6 */

    seq_t res = n6 + (seq_t)2 * n5;   /* n^6 + 2*n^5 */
    res -= (seq_t)5 * n4;             /* - 5*n^4 */
    res += (seq_t)24 * n3;            /* + 24*n^3 */
    res -= (seq_t)41 * n2;            /* - 41*n^2 */
    res += (seq_t)50 * nn;            /* + 50*n */
    res -= (seq_t)31;                 /* - 31 */

    return res;
}

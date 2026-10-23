/*
 * A167549  The sixth row of the ED1 array A167546.
 * https://oeis.org/A167549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 11*n^5 + 209*n^3 - 264*n^2 + 308*n - 144
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 52, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t x  = (seq_t)n;
    seq_t x2 = x * x;          /* n^2 */
    seq_t x3 = x2 * x;         /* n^3 */
    seq_t x5 = x3 * x2;        /* n^5 */

    seq_t res = 11 * x5;       /* 11*n^5 */
    res += 209 * x3;           /* + 209*n^3 */
    res -= 264 * x2;           /* - 264*n^2 */
    res += 308 * x;            /* + 308*n   */
    res -= 144;                /* - 144     */
    return res;
}

/*
 * A168346  a(n) = n^4*(n^10 + 1)/2.
 * https://oeis.org/A168346
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4*(n^10 + 1)/2
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 24, u128 n <= 593,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
seq_t a(idx_t n)
{
    seq_t x     = (seq_t)n;
    seq_t x2    = x * x;          /* n^2 */
    seq_t x4    = x2 * x2;        /* n^4 */
    seq_t x5    = x4 * x;         /* n^5 */
    seq_t x10   = x5 * x5;        /* n^10 */
    seq_t x10p1 = x10 + (seq_t)1; /* n^10 + 1 */

    seq_t res;
    if ((x4 & (seq_t)1) == (seq_t)0) {
        /* n even: n^4 is even, divide it by 2 */
        res = (x4 >> 1) * x10p1;
    } else {
        /* n odd: n^10 + 1 is even, divide that by 2 */
        res = x4 * (x10p1 >> 1);
    }
    return res;
}

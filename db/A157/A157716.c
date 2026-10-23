/*
 * A157716  One-eighth of triangular numbers (integers only).
 * https://oeis.org/A157716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((8*(2*n-1)+7*(-1)^n)^2 - 1) / 64
 * Range:     u8 n <= 8, u16 n <= 128, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* t = 2*n - 1 */
    seq_t t = (seq_t)n * 2 - 1;
    /* eight_t = 8*t */
    seq_t eight_t = t << 3;                 /* 8*t */

    /* tmp = 8*t + 7  if n even, else 8*t - 7  */
    __int128 tmp;
    if (n & 1ULL) {                         /* n is odd */
        tmp = (__int128)eight_t - 7;
    } else {                                /* n is even */
        tmp = (__int128)eight_t + 7;
    }

    /* square and subtract 1 */
    __int128 sq = tmp * tmp;
    sq -= 1;

    /* divide by 64 (shift right by 6 bits) */
    seq_t result = (seq_t)(sq >> 6);
    return result;
}

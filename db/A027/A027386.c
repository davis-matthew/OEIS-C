/*
 * A027386  Write digits for n, count endpoints (version 1).
 * https://oeis.org/A027386
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of endpoint counts of the decimal digits of n. The
 *            endpoint count for a single digit is: 0,8 -> 0 6,9 -> 1
 *            1,2,3,4,5,7 -> 2 The sequence satisfies a(10*n + d) = a(n) +
 *            endpoint(d)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* For completeness, but seq_t/idx_t are provided by the build */

seq_t a(idx_t n)
{
    /* Mapping of endpoints per digit (0-9). */
    static const unsigned char endpoints[10] = {
        0, /* 0 */
        2, /* 1 */
        2, /* 2 */
        2, /* 3 */
        2, /* 4 */
        2, /* 5 */
        1, /* 6 */
        2, /* 7 */
        0, /* 8 */
        1  /* 9 */
    };

    seq_t result = 0;
    idx_t m = n;               /* Work on a copy of the index */

    /* Special case: n == 0 yields 0, which the loop below also provides
       (since the loop will not execute). */
    while (m > 0) {
        result += (seq_t)endpoints[m % 10];
        m /= 10;
    }
    return result;
}

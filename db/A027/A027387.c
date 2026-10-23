/*
 * A027387  Write digits for n, count endpoints (version 2).
 * https://oeis.org/A027387
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of endpoint values of each decimal digit of n where
 *            endpoints are: 0;8 → 0, 6;9 → 1, 1;2;3;5;7 → 2, 4 → 4
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    static const unsigned char endpoints[10] = {
        0, /* 0 */
        2, /* 1 */
        2, /* 2 */
        2, /* 3 */
        4, /* 4 */
        2, /* 5 */
        1, /* 6 */
        2, /* 7 */
        0, /* 8 */
        1  /* 9 */
    };

    seq_t sum = 0;
    while (n != 0) {
        unsigned digit = (unsigned)(n % 10);
        sum += endpoints[digit];
        n /= 10;
    }
    return sum;
}

/*
 * A027389  Write decimal digits for n, count endpoints (version 4).
 * https://oeis.org/A027389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{digits d of n} endpoint[d] endpoint[0] = 0,
 *            endpoint[1] = 2, endpoint[2] = 2, endpoint[3] = 2, endpoint[4] =
 *            4, endpoint[5] = 2, endpoint[6] = 1, endpoint[7] = 4,
 *            endpoint[8] = 0, endpoint[9] = 1
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    static const uint8_t ep[10] = {
        0u, /* 0 */
        2u, /* 1 */
        2u, /* 2 */
        2u, /* 3 */
        4u, /* 4 */
        2u, /* 5 */
        1u, /* 6 */
        4u, /* 7 */
        0u, /* 8 */
        1u  /* 9 */
    };
    seq_t sum = 0;
    while (n > 0) {
        uint8_t d = n % 10;
        sum += ep[d];
        n /= 10;
    }
    return sum;
}

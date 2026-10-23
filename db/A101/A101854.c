/*
 * A101854  a(n) = n*(n+1)*(n^2 + 21*n + 50)/24.
 * https://oeis.org/A101854
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n*(n+1)*(n^2 + 21*n + 50)/24
 * Range:     u8 n <= 5, u16 n <= 30, u32 n <= 561, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   small_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/small_gcd.h"

seq_t a(idx_t n) {
    seq_t nn = n;
    seq_t part1 = nn;                 /* n */
    seq_t part2 = nn + 1;             /* n + 1 */
    seq_t part3 = nn * nn + 21 * nn + 50; /* n^2 + 21n + 50 */

    uint64_t div = 24;                /* denominator */

    /* Reduce each part by factors of the denominator */
    uint64_t g = small_gcd((uint64_t)part1, div);
    part1 /= g;
    div /= g;

    g = small_gcd((uint64_t)part2, div);
    part2 /= g;
    div /= g;

    g = small_gcd((uint64_t)part3, div);
    part3 /= g;
    div /= g;

    /* div should now be 1 */
    return part1 * part2 * part3;
}

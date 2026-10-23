/*
 * A141074  a(n) = n^(n+1)-(n+1)^n+1-(-1)^prime(n+1)-(-1)^(n+1).
 * https://oeis.org/A141074
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n^(n+1)-(n+1)^n+1-(-1)^prime(n+1)-(-1)^(n+1)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* high = n^(n+1) + 1 */
    seq_t high = pow_uint((uint64_t)n, n + 1);
    high += 1;

    /* -(-1)^prime(n+1) */
    if (n == 0) {
        /* prime(1) = 2 → (-1)^2 = 1 → subtract 1 */
        high -= 1;
    } else {
        /* odd prime → (-1)^odd = -1 → subtract(-1) = +1 */
        high += 1;
    }

    /* -(-1)^(n+1) */
    if ((n & 1) == 0) {
        /* n even → n+1 odd → (-1)^odd = -1 → add 1 */
        high += 1;
    } else {
        /* n odd → n+1 even → subtract 1 */
        high -= 1;
    }

    /* low = (n+1)^n */
    seq_t low = pow_uint((uint64_t)(n + 1), n);

    return high - low;
}

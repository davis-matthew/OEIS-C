/*
 * A135534  a(1) = 1; for n>=1, a(2n) = A135561(n), a(2n+1) = 0.
 * https://oeis.org/A135534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   A135534 a(1) = 1; for n >= 1, a(2n) = A135561(n), a(2n+1) = 0
 *            where A135561(1) = 3; A135561(m) = 1 if m is odd > 1; A135561(m)
 *            = 2^{v+2} - 1 for even m with odd part 1 (m = 2^{v}); A135561(m)
 *            = 2^{v+1} - 1 for other even m (odd part > 1), v = exponent of
 *            the largest power of 2 dividing m
 * Range:     u8 n <= 255, u16 n <= 8730, u32 n <= 8730, u64 n <= 8730,
 *            u128 n <= 8730, big any n
 * Helpers:   A135561
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/A135561.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    if (n & 1)            /* odd index > 1 */
        return (seq_t)0;

    return A135561(n >> 1);   /* n = 2 * m, m = n/2 */
}

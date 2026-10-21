/*
 * A087275  Write n in binary: 1ab..yz, then a(n) = 1b..yz + ... + 1yz + 1z +
 *          1.
 * https://oeis.org/A087275
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0, a(2n) = 2*a(n) + 1, a(2n+1) = 2*a(n) + floor(log₂(n))
 *            + 1 (for n ≥ 1)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   _floor_log2_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/_floor_log2_idx.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;      /* a(1) = 0 */

    if ((n & 1) == 0) {               /* n is even: n = 2 * (n/2) */
        idx_t m = n >> 1;
        return (seq_t)(2 * a(m) + 1);
    } else {                           /* n is odd: n = 2 * m + 1 */
        idx_t m = n >> 1;              /* m = (n-1)/2 */
        return (seq_t)(2 * a(m) + _floor_log2_idx(m) + 1);
    }
}

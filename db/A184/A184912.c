/*
 * A184912  n+[ns/r]+[nt/r]+[nu/r], where []=floor and r=2^(1/5), s=r^2,
 *          t=r^3, u=r^4.
 * https://oeis.org/A184912
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n*2^(1/5)) + floor(n*2^(2/5)) +
 *            floor(n*2^(3/5))
 * Range:     u8 n <= 51, u16 n <= 120, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Helpers:   fifth_root_floor_uint128, floor_scale
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <limits.h>
#include "../utils/fifth_root_floor_uint128.h"
#include "../utils/floor_scale.h"

seq_t a(idx_t n) {
    /* a is defined for n >= 1 */
    if (n == 0)
        return (seq_t)0; /* Not part of the sequence but harmless */

    uint64_t rn = (uint64_t)n;
    uint64_t r1 = floor_scale(rn, 1); /* floor(n * 2^(1/5))   */
    uint64_t r2 = floor_scale(rn, 2); /* floor(n * 2^(2/5))   */
    uint64_t r3 = floor_scale(rn, 3); /* floor(n * 2^(3/5))   */

    seq_t result = (seq_t)(rn + r1 + r2 + r3);
    return result;
}

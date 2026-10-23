/*
 * A183218  Complement of the hexagonal numbers.
 * https://oeis.org/A183218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + Floor[1/2 + sqrt(n/2)]
 * Range:     u8 n <= 244, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   uint64_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/uint64_isqrt_u64.h"

seq_t a(idx_t n)
{
    /* n is 1-based index, n >= 1 */
    idx_t n2 = n / 2;                     /* floor(n/2) */
    idx_t k  = uint64_isqrt_u64(n2);      /* floor(sqrt(n/2)) */

    /* Determine whether to add 1 to k.  Condition:
       (k + 0.5)^2 <= n/2   <=> 4*k^2 + 4*k + 1 <= 2*n   */
    unsigned __int128 kk   = (unsigned __int128)k;
    unsigned __int128 lhs  = 4 * kk * kk + 4 * kk + 1;
    unsigned __int128 rhs  = (unsigned __int128)2 * n;
    unsigned add = (lhs <= rhs) ? 1 : 0;

    return (seq_t)n + (seq_t)(k + add);
}

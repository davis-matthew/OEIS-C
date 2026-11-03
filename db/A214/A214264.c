/*
 * A214264  Expansion of f(x^3, x^5) in powers of x where f() is Ramanujan's
 *          two-variable theta function.
 * https://oeis.org/A214264
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if n = k*(4*k+1) for some integer k, otherwise 0
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 103 terms through n = 927 (2026-11-01)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n)
{
    /* D = 1 + 16*n (may need 128 bits) */
    unsigned __int128 D = (unsigned __int128)1 + (unsigned __int128)16 * (unsigned __int128)n;

    /* s = floor(sqrt(D)) */
    unsigned __int128 s = isqrt_u128(D);

    /* Must be a perfect square */
    if (s * s != D)
        return (seq_t)0;

    /* s must be 1 or 7 mod 8 */
    bool ok = ((s & 7) == 1) || ((s & 7) == 7);

    return ok ? (seq_t)1 : (seq_t)0;
}

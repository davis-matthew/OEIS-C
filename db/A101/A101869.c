/*
 * A101869  Row 2 of A101866.
 * https://oeis.org/A101869
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*n + 4*ceiling(n*(1 + sqrt(5))/2)
 * Range:     u8 n <= 29, u16 n <= 7735, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 73 terms through n = 9238 (2026-11-01)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*       = 2*n + 4*(floor(n*phi)+1), phi = (1+sqrt(5))/2 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n)
{
    /* compute floor(n*sqrt(5)) = floor(sqrt(5*n*n)) */
    unsigned __int128 nn = n;
    unsigned __int128 big = nn * nn * 5;              /* 5*n*n */
    unsigned __int128 sqrt_term = isqrt_u128(big);      /* floor(n*sqrt5) */

    /* floor(n*phi) = floor( (n + floor(n*sqrt5))/2 ) */
    unsigned __int128 floor_phi = (nn + sqrt_term) >> 1;

    /* a(n) = 2*n + 4*floor(n*phi) + 4 */
    unsigned __int128 result = (unsigned __int128)2 * nn
                              + (unsigned __int128)4 * floor_phi
                              + 4ULL;

    return (seq_t)result;
}

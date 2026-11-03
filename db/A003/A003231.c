/*
 * A003231  a(n) = floor(n*(sqrt(5)+5)/2).
 * https://oeis.org/A003231
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n + A000201(n), with A000201(n) = floor(n*phi) =
 *            floor((n + isqrt(5*n^2))/2)
 * Range:     u8 n <= 58, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Validated: partial, pass, 58 terms through n = 58 (2026-11-01)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n)
{
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 s = isqrt_u128(5 * N * N);
    unsigned __int128 b = (N + s) / 2;   /* floor(n*phi) */
    unsigned __int128 r = 2 * N + b;
    return (seq_t)(uint64_t)r;
}

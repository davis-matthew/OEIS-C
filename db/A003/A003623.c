/*
 * A003623  Wythoff AB-numbers: floor(floor(n*phi^2)*phi), where phi =
 *          (1+sqrt(5))/2.
 * https://oeis.org/A003623
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*floor(n*phi) + n, with floor(n*phi) = floor((n +
 *            isqrt(5*n^2))/2)
 * Range:     u8 n <= 60, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9401 (2026-11-01)
 * Helpers:   isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n)
{
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 s = isqrt_u128(5 * nn * nn);
    uint64_t fl = (uint64_t)((nn + s) / 2); /* floor(n*phi) */
    seq_t r = (seq_t)fl;
    r = r * 2 + (seq_t)n;
    return r;
}

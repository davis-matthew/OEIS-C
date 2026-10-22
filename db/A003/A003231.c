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
 * Helpers:   a003231_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a003231_isqrt.h"

seq_t a(idx_t n)
{
    unsigned __int128 N = (unsigned __int128)n;
    unsigned __int128 s = a003231_isqrt(5 * N * N);
    unsigned __int128 b = (N + s) / 2;   /* floor(n*phi) */
    unsigned __int128 r = 2 * N + b;
    return (seq_t)(uint64_t)r;
}

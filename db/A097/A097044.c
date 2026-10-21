/*
 * A097044  a(n) = n + a(floor(sqrt(n))) for n > 1; a(1) = 1.
 * https://oeis.org/A097044
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + a(floor(sqrt(n))) (a(1) = 1)
 * Range:     u8 n <= 236, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS sequence A097044 recursively. */
#include <stdint.h>
#include "../utils/floor_isqrt_u64.h"

/* Recursive definition of a(n).  Uses seq_t for the result to keep
 * the result type correct for all builds (uint8_t …,uint64_t,
 * unsigned __int128 or arbitrary precision class).  */
seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;
    idx_t r = floor_isqrt_u64(n);
    return (seq_t)n + a(r);
}

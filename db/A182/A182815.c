/*
 * A182815  The third row of table A182630.
 * https://oeis.org/A182815
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + 3 - i*(i+1)/2, where i =
 *            floor((floor(sqrt(8n+1))–1)/2) – 2 (derived from the definition
 *            of A182630(2,n))
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   floor_sqrt_u128_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt_u128_uint64.h"

seq_t a(idx_t n)
{
    /* Compute j = floor((sqrt(8n+1) – 1)/2) */
    uint64_t j = (floor_sqrt_u128_uint64(n) - 1) / 2;

    /* i may be negative for small n */
    int64_t i = (int64_t)j - 2;

    /* triangular number i*(i+1)/2, use signed __int128 for safety */
    __int128 tri = (__int128)i * (i + 1) / 2;

    /* diff = 3 – tri  (can be negative) */
    __int128 diff = 3 - tri;

    /* final result: n + diff  (non‑negative for all n) */
    __int128 result = (__int128)n + diff;

    return (seq_t)result;
}

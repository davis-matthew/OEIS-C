/*
 * A079633  a(n) = floor(n/floor(n^(1/3))) - floor(n^(2/3)).
 * https://oeis.org/A079633
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n / floor(n^(1/3))) - floor(n^(2/3))
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   cube_root_floor_uint128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cube_root_floor_uint128.h"

/* OEIS A079633: a(n) = floor(n / floor(n^(1/3))) - floor(n^(2/3)) */
seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset starts at 1, but guard anyway */

    __uint128_t n128 = n;               /* for 128‑bit arithmetic */
    uint64_t c = cube_root_floor_uint128(n128);  /* c = ⌊n^(1/3)⌋ */
    __uint128_t n2   = n128 * n128;     /* n² fits in 128 bits */
    uint64_t d = cube_root_floor_uint128(n2);     /* d = ⌊n^(2/3)⌋ */

    /* floor(n / c) is integer division of idx_t (uint64_t). */
    uint64_t div = n / c;
    return (seq_t)(div - d);
}

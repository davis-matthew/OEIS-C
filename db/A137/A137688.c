/*
 * A137688  2^A003056: 2^n appears n+1 times.
 * https://oeis.org/A137688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = 2^( floor( (sqrt(8*n + 1) - 1) / 2 ) ) Sequence: 2^n
 *            appears n+1 times
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 2079,
 *            u128 n <= 5049, big any n
 * Validated: standard, pass, 77 terms through n = 4444 (2026-11-01)
 * Helpers:   isqrt_u128_to_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/isqrt_u128_to_idx.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* 8*n + 1 may overflow 64 bits; use 128‑bit. */
    __uint128_t t = (__uint128_t)n * 8 + 1;
    idx_t sqrt_floor = isqrt_u128_to_idx(t);
    /* k = floor((sqrt_floor - 1) / 2) */
    idx_t k = (sqrt_floor - 1) / 2;
    return pow2(k);
}

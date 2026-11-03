/*
 * A196305  a(n) = 15*2^n - 1.
 * https://oeis.org/A196305
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 15*2^n - 1
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 191 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"

/* multiply a seq_t by a small uint32_t constant without overflow
 * for 64‑bit seq_t we use unsigned __int128 as intermediate,
 * for larger seq_t we rely on the built‑in arithmetic. */
static inline seq_t mul_small(seq_t x, unsigned int m)
{
    if (sizeof(seq_t) <= 8) {
        unsigned __int128 res = (unsigned __int128)x * m;
        return (seq_t)res;
    } else {
        return x * m;
    }
}

seq_t a(idx_t n)
{
    seq_t pow = pow2(n);            /* 2^n   */
    seq_t prod = mul_small(pow, 15u);  /* 15 * 2^n */
    return prod - 1;
}

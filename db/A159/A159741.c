/*
 * A159741  a(n) = 8*(2^n - 1).
 * https://oeis.org/A159741
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8*(2^n - 1)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 997 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    if (n == 0) /* sequence is defined for n ≥ 1; return 0 for safety */
        return (seq_t)0;

    seq_t p = pow2(n);                 /* 2^n */
    seq_t res = mul_small(p, 8);       /* 8 * 2^n */
    res -= (seq_t)8;                   /* subtract 8 → 8*(2^n - 1) */
    return res;
}

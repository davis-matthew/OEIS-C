/*
 * A019333  Expansion of g.f. 1/((1-4*x)*(1-6*x)*(1-8*x)).
 * https://oeis.org/A019333
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*4^n - 9*6^n + 8*8^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 20, u128 n <= 41,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 197 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/ipow_i32.h"

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
    /* 2 * 4^n */
    seq_t t4 = ipow_i32((seq_t)4, n);
    t4 = mul_small(t4, 2);

    /* 9 * 6^n */
    seq_t t6 = ipow_i32((seq_t)6, n);
    t6 = mul_small(t6, 9);

    /* 8 * 8^n */
    seq_t t8 = ipow_i32((seq_t)8, n);
    t8 = mul_small(t8, 8);

    /* a(n) = (2*4^n) + (8*8^n) - (9*6^n) */
    seq_t r = t4 + t8;
    r -= t6;
    return r;
}

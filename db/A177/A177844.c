/*
 * A177844  a(n) = 6*a(n-1)-8*a(n-2) for n > 3; a(0)=279, a(1)=3996,
 *          a(2)=16008, a(3)=64784.
 * https://oeis.org/A177844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^(n+5)-47*2^(n+1) for n>1; a(0)=279, a(1)=3996
 * Range:     u8 none, u16 n <= 3, u32 n <= 11, u64 n <= 27, u128 n <= 59,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 936 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/pow2.h"

static inline seq_t pow4(idx_t exp) {
    seq_t result = 1;
    seq_t base   = 4;
    idx_t e = exp;
    while (e) {
        if (e & 1) result *= base;
        base *= base;
        e >>= 1;
    }
    return result;
}

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
    if (n == 0) return (seq_t)279;
    if (n == 1) return (seq_t)3996;

    /* For n >= 2 use the closed form */
    seq_t term1 = pow4((idx_t)(n + 5));               /* 4^(n+5) */
    seq_t term2 = mul_small(pow2((idx_t)(n + 1)), 47);/* 47*2^(n+1) */
    return term1 - term2;
}

/*
 * A083587  a(n) = 4*3^n/3 - 5*0^n/6 - (n-1)2^(n-1).
 * https://oeis.org/A083587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*3^(n-1) - (n-1)*2^(n-1) for n>=1, a(0)=1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 990 (2026-11-01)
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

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
    if (n == 0)
        return (seq_t)1;

    /* 4 * 3^(n-1) */
    seq_t p3 = pow3(n - 1);
    seq_t term1 = mul_small(p3, 4);

    /* (n-1) * 2^(n-1) */
    seq_t p2 = pow2(n - 1);
    seq_t term2 = mul_small(p2, (unsigned int)(n - 1));

    return term1 - term2;
}

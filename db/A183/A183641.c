/*
 * A183641  Number of (n+1) X 9 0..3 arrays with every 2 X 2 subblock summing
 *          to 6.
 * https://oeis.org/A183641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*4^n + 1530*3^n + 37320*2^n + 223290
 * Range:     u8 none, u16 none, u32 n <= 13, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 997 (2026-11-01)
 * Helpers:   pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow3.h"

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
    /* n ≥ 1 */
    seq_t p4 = pow4(n);          /* 4^n   */
    seq_t p3 = pow3(n);          /* 3^n   */
    seq_t p2 = pow2(n);          /* 2^n   */
    seq_t result = mul_small(p4, 4);          /* 4*4^n   */
    result += mul_small(p3, 1530);           /* 1530*3^n*/
    result += mul_small(p2, 37320);          /* 37320*2^n */
    result += (seq_t)223290;                /* constant   */
    return result;
}

/*
 * A186947  a(n) = 4^n - n*2^n.
 * https://oeis.org/A186947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^n - n*2^n
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Validated: standard, pass, 42 terms through n = 903 (2026-11-01)
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
    seq_t pow2_n = pow2(n);                 /* 2^n                       */
    seq_t pow4_n = pow2(n << 1);            /* 4^n (i.e. 2^(2n))         */
    seq_t n_mul  = mul_small(pow2_n, (unsigned int)n); /* n*2^n   */
    return pow4_n - n_mul;                  /* 4^n - n*2^n               */
}

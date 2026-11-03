/*
 * A169382  Number of reduced words of length n in Coxeter group on 33
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^31 = I.
 * https://oeis.org/A169382
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = 33 * 32^(n-1)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 15,
 *            big any n
 * Validated: partial, pass, 16 terms through n = 15 (2026-11-01)
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

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }
    idx_t exp = 5 * ((idx_t)n - 1);  /* 32^(n-1) = 2^(5*(n-1)) */
    seq_t p = pow2(exp);             /* p = 2^exp = 32^(n-1) */
    return mul_small(p, 33u);        /* a(n) = 33 * p */
}

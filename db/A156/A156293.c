/*
 * A156293  Denominator of Euler(n, 1/11).
 * https://oeis.org/A156293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 2^v2(n+1) * 11^n (v2 is the exponent of 2 in n+1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   ipow_idx, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_idx.h"
#include "../utils/pow2.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* 11^n */
    seq_t pow11 = ipow_idx((seq_t)11, n);

    /* compute v2(n+1) */
    idx_t np1 = n + 1;
    idx_t v   = 0;
    while ((np1 & 1) == 0) {
        v++;
        np1 >>= 1;
    }

    /* 2^v */
    seq_t pow2v = pow2(v);

    /* product */
    return seq_mul(pow11, pow2v);
}

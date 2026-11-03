/*
 * A122551  Denominators of the coefficients of the series for InverseErf(x).
 * https://oeis.org/A122551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2*n+1)!*2^(n+1)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Validated: standard, pass, 29 terms through n = 210 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

static inline seq_t seq_one(void) { return (seq_t)1; }

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

/* Compute the denominators of the coefficients of the series for InverseErf(x) */
seq_t a(idx_t n)
{
    /* compute (2*n+1)! */
    seq_t fac = seq_one();                     /* start with 1 */
    idx_t limit = 2*n + 1;                     /* 2*n+1 fits in idx_t for intended use */
    for (idx_t i = 2; i <= limit; ++i)
        fac = seq_mul(fac, (seq_t)i);          /* fac *= i */

    /* compute 2^(n+1) */
    seq_t pow2 = ipow_idx((seq_t)2, n + 1);

    /* result = fac * pow2 */
    return seq_mul(fac, pow2);
}

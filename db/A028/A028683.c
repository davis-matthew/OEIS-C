/*
 * A028683  Pseudo Galois numbers for d=21.
 * https://oeis.org/A028683
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 21^n * ∏_{k=0}^{n-1} (21^n - 21^k)
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 4,
 *            big any n
 * Validated: standard, pass, 24 terms through n = 27 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalently a(n) = 21^{n(n+1)/2} * ∏_{t=1}^n (21^t - 1)) */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    /* Compute 21^{n(n+1)/2} */
    idx_t exp = n * (n + 1) / 2;
    seq_t powExp = ipow_idx((seq_t)21, exp);

    /* Compute ∏_{t=1}^n (21^t - 1) */
    seq_t powLoop = (seq_t)1;
    seq_t product = (seq_t)1;
    for (idx_t t = 1; t <= n; ++t) {
        powLoop = seq_mul(powLoop, (seq_t)21);   /* powLoop = 21^t */
        seq_t term = powLoop - (seq_t)1;          /* 21^t - 1 */
        product = seq_mul(product, term);
    }

    /* a(n) = powExp * product */
    return seq_mul(powExp, product);
}

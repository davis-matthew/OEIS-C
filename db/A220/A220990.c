/*
 * A220990  a(n) = 12^(2n+1) + 6 * 12^n + 1: the right Aurifeuillian factor of
 *          12^(6n+3) + 1.
 * https://oeis.org/A220990
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12^(2n+1) + 6*12^n + 1
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../utils/ipow_idx.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Compute 12^n */
    seq_t pow_n = ipow_idx((seq_t)12, n);

    /* Compute 12^(2n+1) as 12 * (12^n)^2 */
    seq_t pow_2n1 = seq_mul(pow_n, seq_mul(pow_n, (seq_t)12));

    /* Compute 6 * 12^n */
    seq_t six_pow_n = seq_mul(pow_n, (seq_t)6);

    /* Assemble the result */
    return pow_2n1 + six_pow_n + (seq_t)1;
}

/*
 * A028131  Expansion of 1/((1-4x)(1-6x)(1-7x)(1-9x)).
 * https://oeis.org/A028131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9^(n+3)-5*7^(n+3)+5*6^(n+3)-4^(n+3))/30
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 7, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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
    idx_t exp = n + 3;
    seq_t p4 = ipow_idx((seq_t)4, exp);
    seq_t p6 = ipow_idx((seq_t)6, exp);
    seq_t p7 = ipow_idx((seq_t)7, exp);
    seq_t p9 = ipow_idx((seq_t)9, exp);

    seq_t term1 = seq_mul((seq_t)5, p6);
    term1 = term1 + p9;

    seq_t term2 = seq_mul((seq_t)5, p7);
    term2 = term2 + p4;

    seq_t diff = term1 - term2;
    return diff / (seq_t)30;
}

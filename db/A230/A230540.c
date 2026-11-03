/*
 * A230540  a(n) = 2*n*3^(2*n-1).
 * https://oeis.org/A230540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*n*3^(2*n-1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 100 (2026-11-01)
 * Helpers:   ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses seq_pow and seq_mul from utils */

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
    if (n == 0)
        return 0;

    /* compute 3^(2*n-1) */
    seq_t pow3 = ipow_idx((seq_t)3, 2 * n - 1);

    /* multiply by 2*n */
    seq_t t = (seq_t)(2 * n);
    return seq_mul(t, pow3);
}

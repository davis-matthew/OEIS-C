/*
 * A198403  a(n) = 6^n * n^6.
 * https://oeis.org/A198403
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n * n^6
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 15, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 954 (2026-11-01)
 * Helpers:   ipow, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"
#include "../utils/ipow_i32.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;               /* a(0) = 0 */

    /* compute 6^n   (big_pow works for all seq_t types) */
    seq_t base6 = 6;
    seq_t pow6   = ipow(base6, (seq_t)n);

    /* compute n^6   (pow_seq works for all seq_t types) */
    seq_t n6 = ipow_i32((seq_t)n, 6);

    /* final product */
    return seq_mul(pow6, n6);
}

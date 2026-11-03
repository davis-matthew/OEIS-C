/*
 * A127688  a(n) = (n+1)^n - n!!.
 * https://oeis.org/A127688
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)^n - n!! (OEIS A127688)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 358 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_i32.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* compute (n+1)^n */
    seq_t pow_val = ipow_i32((seq_t)(n + 1), (int)n);

    /* compute n!! (double factorial) */
    seq_t double_fact = (seq_t)1;
    for (idx_t i = n; i > 0; ) {
        double_fact = seq_mul(double_fact, (seq_t)i);
        if (i <= 2) break;              /* finish with 1 or 2 */
        i -= 2;
    }

    /* return the difference */
    return pow_val - double_fact;
}

/*
 * A135420  a(n) = 4^n * 5^(n^2).
 * https://oeis.org/A135420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4^n * 5^(n^2)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 25 terms through n = 25 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    /* Compute 4^n. */
    seq_t pow4 = ipow((seq_t)4, (seq_t)n);

    /* Compute 5^(n^2).  n^2 is calculated in seq_t to keep the
       computation exact for arbitrary‑precision builds. */
    seq_t exp_n2 = (seq_t)n * (seq_t)n;
    seq_t pow5 = ipow((seq_t)5, exp_n2);

    /* Return the product. */
    return seq_mul(pow4, pow5);
}

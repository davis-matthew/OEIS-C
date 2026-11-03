/*
 * A199116  a(n) = 6*4^n + 1.
 * https://oeis.org/A199116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*4^n + 1
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 30, u128 n <= 62,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 941 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* Compute 4^n using fast exponentiation (big_pow works for all
       integer widths, including arbitrary‑precision types). */
    seq_t pow4 = ipow((seq_t)4, (seq_t)n);

    /* Multiply by 6 and add 1 to get 6*4^n + 1. */
    seq_t result = pow4 * (seq_t)6 + (seq_t)1;
    return result;
}

/*
 * A024064  a(n) = 6^n - n^2.
 * https://oeis.org/A024064
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n - n^2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 36 terms through n = 431 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* 6^n */
    seq_t pw = ipow((seq_t)6, (seq_t)n);
    /* n^2 */
    seq_t nn = ((seq_t)n) * ((seq_t)n);
    return pw - nn;
}

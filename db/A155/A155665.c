/*
 * A155665  a(n) = 11^n + 8^n - 1.
 * https://oeis.org/A155665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n + 8^n - 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 919 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow11 = ipow((seq_t)11, (seq_t)n);
    seq_t pow8  = ipow((seq_t)8,  (seq_t)n);
    seq_t res   = pow11 + pow8;
    res         -= 1;
    return res;
}

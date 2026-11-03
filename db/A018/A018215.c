/*
 * A018215  a(n) = n*4^n.
 * https://oeis.org/A018215
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*4^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 29, u128 n <= 61,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 277 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    /* a(0) = 0; for n>0 compute n * 4^n */
    if (n == 0)
        return (seq_t)0;
    /* 4^n */
    seq_t pow4 = ipow((seq_t)4, (seq_t)n);
    /* n * 4^n */
    return (seq_t)n * pow4;
}

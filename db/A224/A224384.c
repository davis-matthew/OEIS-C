/*
 * A224384  a(n) = 1 + 17^n.
 * https://oeis.org/A224384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 17^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 31,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 197 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)17;
    seq_t exp  = (seq_t)n;
    seq_t pow  = ipow(base, exp);
    return pow + (seq_t)1;
}

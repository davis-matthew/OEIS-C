/*
 * A199319  a(n) = 4*6^n+1.
 * https://oeis.org/A199319
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*6^n+1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 23, u128 n <= 48,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 945 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t pow6 = ipow((seq_t)6, n);
    return pow6 * (seq_t)4 + (seq_t)1;
}

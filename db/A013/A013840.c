/*
 * A013840  a(n) = 6^(5*n + 3).
 * https://oeis.org/A013840
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^(5*n+3)
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 1, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Validated: standard, pass, 26 terms through n = 100 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t exp = (seq_t)n * (seq_t)5 + (seq_t)3;
    return ipow((seq_t)6, exp);
}

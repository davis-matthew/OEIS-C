/*
 * A199750  a(n) = 2*11^n+1.
 * https://oeis.org/A199750
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*11^n+1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 36,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 870 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 3;                       // a(0) = 3
    seq_t val = ipow((seq_t)11, (seq_t)n); // compute 11^n
    val = val * 2 + 1;                  // 2*11^n + 1
    return val;
}

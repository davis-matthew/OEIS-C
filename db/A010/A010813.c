/*
 * A010813  25th powers: a(n) = n^25.
 * https://oeis.org/A010813
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^25
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 5, u128 n <= 34,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 925 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    return ipow((seq_t)n, (seq_t)25);
}

/*
 * A010811  23rd powers: a(n) = n^23.
 * https://oeis.org/A010811
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, mult, easy
 * Formula:   a(n) = n^23
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 6, u128 n <= 47,
 *            big any n
 * Validated: standard, pass, 27 terms through n = 970 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    return ipow((seq_t)n, (seq_t)23);
}

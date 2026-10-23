/*
 * A192450  Numbers k such that -1 is not a square mod k.
 * https://oeis.org/A192450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 202, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_p3mod4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A192450: numbers k such that -1 is not a square mod k.
   A term is included if k is divisible by 4 or if k has a prime factor
   congruent to 3 (mod 4).  The function enumerates k successively
   starting at 3 and returns the nth such number (offset 1). */
#include <stdint.h>
#include <stdbool.h>
#include "../utils/has_p3mod4.h"

seq_t a(idx_t n)
{
    uint64_t count = 0;
    for (uint64_t k = 3ULL; ; ++k) {
        if ((k & 3ULL) == 0ULL || has_p3mod4(k)) {
            ++count;
            if (count == n) return (seq_t)k;
        }
    }
}

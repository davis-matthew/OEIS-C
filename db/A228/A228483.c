/*
 * A228483  Duplicate of A129979.
 * https://oeis.org/A228483
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 2 - mu(n) (where mu is the Möbius function)
 * Range:     i8 n <= 1001, i16 n <= 1001, i32 n <= 1001, i64 n <= 1001,
 *            i128 n <= 1001, big any n
 * Validated: standard, pass, 103 terms through n = 879 (2026-11-01)
 * Helpers:   mobius
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mobius.h"

seq_t a(idx_t n)
{
    /* n ≥ 1, a(n) = 2 - mobius(n) */
    return (seq_t)2 - mobius(n);
}

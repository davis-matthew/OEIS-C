/*
 * A084327  Duplicate of A057085.
 * https://oeis.org/A084327
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = 9*a(n-1) - 9*a(n-2), a(0) = 0, a(1) = 1
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 11, i64 n <= 11, i128 n <= 11,
 *            big any n
 * Validated: partial, pass, 12 terms through n = 11 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    seq_t p = 0, q = 1;   /* a(0), a(1) */
    if (n == 0)
        return p;
    for (idx_t i = 1; i < n; ++i) {
        seq_t t = 9 * q - 9 * p;
        p = q;
        q = t;
    }
    return q;
}

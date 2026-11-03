/*
 * A123570  Duplicate of A110567.
 * https://oeis.org/A123570
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n^(n+1) + 1
 * Range:     i8 n <= 3, i16 n <= 5, i32 n <= 8, i64 n <= 15, i128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-01)
 * Helpers:   ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)n;
    seq_t exp  = (seq_t)(n + 1);
    seq_t val  = ipow(base, exp);
    val += (seq_t)1;
    return val;
}

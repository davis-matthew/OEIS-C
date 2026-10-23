/*
 * A138332  C(n+7, 7)*(n+4)*(-1)^(n+1)*16.
 * https://oeis.org/A138332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = C(n+7,7)*(n+4)*(-1)^(n+1)*16
 * Range:     i8 n <= 0, i16 n <= 3, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    seq_t bin = binom(n + 7, 7);          /* C(n+7,7)  */
    seq_t val = bin * (seq_t)(n + 4) * (seq_t)16;  /* * (n+4) * 16  */
    if ((n + 1u) & 1u)                   /* apply (-1)^(n+1)  */
        val = -val;
    return val;
}

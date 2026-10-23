/*
 * A023391  a(n+1) = a(n) converted to base 9 from base 8 (written in base
 *          10).
 * https://oeis.org/A023391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = a(n‐1) converted from base 8 to base 9, * a(1) = 8. The
 *            conversion interprets the digits of a(n‑1) * in base 8, builds a
 *            number in base 9, and outputs that * number in base 10
 * Range:     u8 n <= 28, u16 n <= 44, u32 n <= 57, u64 n <= 71, u128 n <= 83,
 *            big any n
 * Helpers:   base8_to_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/base8_to_base9.h"

seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)8;   /* a(1) = 8 */

    seq_t x = 8;
    for (idx_t i = 1; i < n; ++i) {
        x = base8_to_base9(x);
    }
    return x;
}

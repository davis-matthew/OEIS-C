/*
 * A128960  a(n) = (n^3 - n)*2^n.
 * https://oeis.org/A128960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^3 - n)*2^n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 19, u64 n <= 47, u128 n <= 107,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 869 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    return (nn * nn * nn - nn) * pow2(n);
}

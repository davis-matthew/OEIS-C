/*
 * A048495  a(n) = (n-1)*2^n + 2.
 * https://oeis.org/A048495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) * 2^n + 2
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 27, u64 n <= 58, u128 n <= 121,
 *            big any n
 * Validated: standard, pass, 49 terms through n = 986 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    seq_t pow2n = pow2(n);
    return (seq_t)(n - 1) * pow2n + (seq_t)2;
}

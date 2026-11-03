/*
 * A061761  a(n) = 2^n + 2*n - 1.
 * https://oeis.org/A061761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^n + 2*n - 1
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 184 (2026-11-01)
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t result = pow2(n);          /* 2^n */
    result += (seq_t)n * 2;          /* + 2*n */
    result -= 1;                    /* - 1 */
    return result;
}

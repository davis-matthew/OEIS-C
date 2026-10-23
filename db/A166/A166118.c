/*
 * A166118  Fixed points of the mapping f(x) = (x + 2^x) mod (17 + x).
 * https://oeis.org/A166118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*2^n - 17, for n >= 1
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 28, u64 n <= 60, u128 n <= 124,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* Not defined for n < 1, return 0 as a safe default */
    seq_t val = pow2(n);          /* Compute 2^n */
    val *= 16;                    /* Multiply by 16: 16*2^n */
    val -= 17;                    /* Subtract 17 */
    return val;
}

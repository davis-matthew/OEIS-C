/*
 * A164285  a(n) = 7*2^n + 3.
 * https://oeis.org/A164285
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7 * 2^n + 3
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 61, u128 n <= 125,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute 2^n, multiply by 7 and add 3. */
    seq_t r = pow2(n);          /* 2^n */
    r = r * (seq_t)7;           /* 7 * 2^n */
    r += (seq_t)3;              /* + 3 */
    return r;
}

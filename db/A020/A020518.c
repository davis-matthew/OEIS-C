/*
 * A020518  10th cyclotomic polynomial evaluated at powers of 2.
 * https://oeis.org/A020518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1-2^n+4^n-8^n+16^n
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 32,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute the required powers of 2 */
    seq_t p2   = pow_uint(2ULL, n);
    seq_t p4   = pow_uint(4ULL, n);
    seq_t p8   = pow_uint(8ULL, n);
    seq_t p16  = pow_uint(16ULL, n);

    /* combine them in a non‑negative order */
    seq_t result = p16;
    result += p4;
    result += (seq_t)1;
    result -= p8;
    result -= p2;

    return result;
}

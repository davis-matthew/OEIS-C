/*
 * A177096  a(n) = 5*(10^n - 2).
 * https://oeis.org/A177096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*(10^n - 2)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   mul_small, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    /* compute 10^n, subtract 2, then multiply by 5 */
    seq_t ten_n   = pow10(n);
    seq_t val     = ten_n - 2;
    seq_t result  = mul_small(val, 5);
    return result;
}

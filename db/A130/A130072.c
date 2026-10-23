/*
 * A130072  a(n) = 5^n - 3^n - 2^n.
 * https://oeis.org/A130072
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n - 3^n - 2^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* For n ≥ 1; a(1) = 0, a(2) = 12, … */
    seq_t pow5 = pow_uint(5, n);
    seq_t pow3 = pow_uint(3, n);
    seq_t pow2 = pow_uint(2, n);

    seq_t result = pow5 - pow3;
    result -= pow2;
    return result;
}

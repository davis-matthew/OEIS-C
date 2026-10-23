/*
 * A156270  a(n) = 8^n*Catalan(n).
 * https://oeis.org/A156270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 8^n * Catalan(n)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   catalan, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_uint.h"
#include "../utils/catalan.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    seq_t pow8 = pow_uint(8, n);
    seq_t cat  = catalan(n);
    return seq_mul(pow8, cat);
}

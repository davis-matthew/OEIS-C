/*
 * A135426  a(n) = 7^n * 8^(n^2).
 * https://oeis.org/A135426
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 7^n * 8^(n^2)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 6,
 *            big any n
 * Helpers:   pow2, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Calculate 7^n */
    seq_t t1 = pow_uint(7ULL, n);

    /* Calculate 8^(n^2) as 2^(3*n^2) */
    uint64_t exp = 3ULL * n * n;          /* fits in uint64_t for n <= 10000 */
    seq_t t2 = pow2(exp);

    /* Multiply the two factors */
    return t1 * t2;
}

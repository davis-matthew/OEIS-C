/*
 * A173123  a(n) = binomial(n+9,9)*6^n.
 * https://oeis.org/A173123
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+9,9)*6^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 37,
 *            big any n
 * Helpers:   binom, pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    seq_t c = binom(n + 9, 9);
    seq_t p = pow_uint(6, n);
    return c * p;
}

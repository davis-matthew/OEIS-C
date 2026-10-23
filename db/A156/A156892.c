/*
 * A156892  Denominator of Euler(n, 1/23).
 * https://oeis.org/A156892
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = 23^n * 2^{v2(n+1)} where v2(k) is the exponent of 2
 *            dividing k
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t val = 1;                     /* start with 23^0 = 1  */
    for (idx_t i = 0; i < n; ++i)      /* compute 23^n */
        val = mul_small(val, (unsigned int)23);

    unsigned int shift = (unsigned int)__builtin_ctzll((uint64_t)n + 1U); /* v2(n+1) */
    for (unsigned int i = 0; i < shift; ++i)                             /* * 2^shift */
        val = mul_small(val, 2);

    return val;
}

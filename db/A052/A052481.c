/*
 * A052481  a(n) = 2^n*(binomial(n,2) + 1).
 * https://oeis.org/A052481
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n*(binomial(n,2) + 1)
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 23, u64 n <= 53, u128 n <= 115,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n < 2U)
        return pow2(n);           /* multiplier is 1 for n = 0,1 */

    seq_t mult = ((seq_t)n * ((seq_t)n - 1)) / 2 + 1;
    return pow2(n) * mult;
}

/*
 * A133133  a(n) is the largest prime factor of the sum of the largest prime
 *          factors of numbers from 2 to n.
 * https://oeis.org/A133133
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = gpf( Σ_{k=2}^n gpf(k) )
 * Range:     u8 n <= 36, u16 n <= 672, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gpf.h"

seq_t a(idx_t n)
{
    /* n ≥ 2 for valid terms, values for n < 2 are undefined but return 0 */
    if (n < 2)
        return 0;

    uint64_t sum = 0;
    for (idx_t k = 2; k <= n; ++k)
        sum += gpf((uint64_t)k);

    return (seq_t)gpf(sum);
}

/*
 * A067342  Sum of decimal digits of sum of divisors of n.
 * https://oeis.org/A067342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum of decimal digits of the sum of divisors of n
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, sigma_of_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/sigma_of_uint64.h"

/* External function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour below the offset is irrelevant */
        return 0;

    seq_t sigma = sigma_of_uint64((uint64_t)n);
    return digit_sum(sigma);
}

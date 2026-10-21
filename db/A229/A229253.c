/*
 * A229253  Total number of elements of nonempty subsets of divisors of n.
 * https://oeis.org/A229253
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = tau(n) * 2^(tau(n)-1)
 * Range:     u8 n <= 23, u16 n <= 119, u32 n <= 719, u64 n <= 5039,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where tau(n) is the number of positive divisors of n */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* tau(n) is at least 1 for n >= 1 */
    idx_t tau = divisor_count_u64(n);

    seq_t pow2 = 1;
    for (idx_t i = 1; i < tau; ++i)
        pow2 <<= 1;   /* shift by one bit each iteration */

    return (seq_t)tau * pow2;
}

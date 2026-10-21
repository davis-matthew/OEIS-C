/*
 * A096736  a(1) = 2; for n>1: a(n) = integer part of x-value when y=0 in
 *          (y-tau(n))/(x-1)=(1-tau(n))/(n-1), tau=A000005.
 * https://oeis.org/A096736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n * tau(n) - 1) / (tau(n) - 1)) for n > 1 , a(1) =
 *            2 where tau(n) is the number of positive divisors of n
 * Range:     u8 n <= 130, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)2;                /* a(1) = 2 */

    /* tau(n) is at least 2 for n > 1, so the denominator is non‑zero */
    uint64_t tau   = divisor_count_u64((uint64_t)n);
    uint64_t num   = (uint64_t)n * tau - 1;
    uint64_t denom = tau - 1;
    return (seq_t)(num / denom);
}

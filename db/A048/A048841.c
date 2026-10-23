/*
 * A048841  Least positive integer k for which 11^n divides k!.
 * https://oeis.org/A048841
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k ≥ 1 such that the exponent of 11 in k! is at
 *            least n
 * Range:     u8 n <= 25, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   exponent_of_11_in_factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/exponent_of_11_in_factorial.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* 11^0 = 1 divides 1! by definition */

    idx_t low  = 0;
    idx_t high = 11 * n;          /* crude upper bound: grows with n */
    if (high == 0)
        high = 1;

    /* Guard against overflow: double until we are sure the exponent is large enough */
    while (exponent_of_11_in_factorial(high) < n)
    {
        if (high > (idx_t)-1 / 2)   /* should never happen for the intended limits */
            break;
        high *= 2;
    }

    /* Binary search for the minimal k with exponent ≥ n */
    while (high - low > 1)
    {
        idx_t mid = low + (high - low) / 2;
        if (exponent_of_11_in_factorial(mid) >= n)
            high = mid;
        else
            low = mid;
    }

    return (seq_t)high;
}

/*
 * A060278  Sum of composite divisors of n less than n.
 * https://oeis.org/A060278
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = sum of composite divisors of n that are less than n
 * Range:     u8 n <= 167, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_small.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    /* Enumerate divisors up to sqrt(n). Any divisor d>1 that is not prime
       is composite, and its paired divisor e = n/d (if different from d)
       is also a divisor. We skip the divisor n itself. */
    for (idx_t d = 2; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* divisor d */
            if (d < n && !is_prime_small((seq_t)d))
                sum += (seq_t)d;

            /* paired divisor e */
            idx_t e = n / d;
            if (e != d && e < n && !is_prime_small((seq_t)e))
                sum += (seq_t)e;
        }
    }
    return sum;
}

/*
 * A138011  a(n) = number of positive divisors, k, of n where d(k) divides
 *          d(n). (d(m) = number of positive divisors of m, A000005).
 * https://oeis.org/A138011
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of divisors k of n such that d(k) | d(n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: Enumerate all divisors up to sqrt(n), compute d(k) by */
/* divisor_count_u64, and count those dividing d(n).                     */

#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    /* d(n) */
    seq_t d_n = divisor_count_u64((uint64_t)n);
    seq_t count = 0;

    /* Iterate over divisors k of n; k <= n/k ensures each divisor is checked once. */
    for (idx_t k = 1; k <= n / k; ++k)
    {
        if (n % k == 0)
        {
            seq_t d_k = divisor_count_u64((uint64_t)k);
            if (d_n % d_k == 0)
                ++count;

            idx_t k2 = n / k;
            if (k2 != k)
            {
                seq_t d_k2 = divisor_count_u64((uint64_t)k2);
                if (d_n % d_k2 == 0)
                    ++count;
            }
        }
    }
    return count;
}

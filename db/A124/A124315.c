/*
 * A124315  a(n) = Sum_{d|n} tau(gcd(d,n/d)), where tau = sigma_0 = A000005.
 * https://oeis.org/A124315
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  mult, nonn, easy
 * Formula:   a(n) = Sum_{d|n} tau(gcd(d,n/d))
 * Range:     u8 n <= 3484, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   divisor_count_u64, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implements the definition directly by enumerating all divisors of n.
   It uses the provided gcd() and divisor_count_u64() utilities. */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/divisor_count_u64.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t d;
    for (d = 1; d <= n / d; ++d)             /* iterate divisor pairs (d, n/d) */
    {
        if (n % d == 0)
        {
            idx_t nd = n / d;
            seq_t g  = gcd((seq_t)d, (seq_t)nd);
            seq_t t  = divisor_count_u64((uint64_t)g);
            sum += t;                         /* for divisor d */

            if (d != nd)                     /* for divisor n/d */
            {
                /* gcd(nd, d) is identical, so reuse or recompute */
                sum += t;
            }
        }
    }
    return sum;
}

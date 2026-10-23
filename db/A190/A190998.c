/*
 * A190998  Digital root of concatenation of all divisors of n (A037278).
 * https://oeis.org/A190998
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = digital_root( ∑_{d|n} digit_sum(d) )
 * Range:     u8 n <= 889, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This is the digital root of the concatenation of all divisors of n.
   Since the sum of the digits of the concatenated string equals the
   sum of the digits of all divisors, we sum digit_sum(d) over all d|n
   and then take its digital root. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* For n < 1 the sequence is undefined; return 0 in that case. */
    if (n < 1)
        return (seq_t)0;

    seq_t sum = (seq_t)0;
    idx_t h;

    for (h = 1; h * h <= n; ++h)
    {
        if (n % h == 0)
        {
            /* First divisor h */
            sum += digit_sum((seq_t)h);

            /* Second divisor n/h, if distinct */
            idx_t other = n / h;
            if (other != h)
                sum += digit_sum((seq_t)other);
        }
    }

    /* Compute digital root: dr(x) = 9 if x % 9 == 0 else x % 9 */
    seq_t mod = sum % (seq_t)9;
    return (mod == 0) ? (seq_t)9 : mod;
}

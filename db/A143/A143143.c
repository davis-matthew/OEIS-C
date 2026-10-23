/*
 * A143143  a(n) = the number of primes that exist between (but don't include)
 *          j and k, where j is the largest divisor of n that is <= sqrt(n)
 *          and k = the smallest divisor of n that is >= sqrt(n).
 * https://oeis.org/A143143
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes strictly between the two middle divisors
 *            of n
 * Range:     u8 n <= 726, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isPrimeTrial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* If n is a perfect square the two middle divisors coincide and the answer is 0. */

#include <stdbool.h>
#include "../utils/isPrimeTrial.h"

seq_t a(idx_t n)
{
    /* safety for accidental zero index */
    if (n == 0) return 0;

    /* compute floor(sqrt(n)) without overflow */
    idx_t sqrt_n = 0;
    for (idx_t i = 1; i <= n / i; ++i)    /* n / i keeps the product from overflowing */
        sqrt_n = i;

    /* find largest divisor <= sqrt(n) */
    idx_t j = 1;
    for (idx_t d = sqrt_n; d >= 1; --d)
    {
        if (n % d == 0)
        {
            j = d;
            break;
        }
    }

    /* corresponding smallest divisor >= sqrt(n) */
    idx_t k = n / j;   /* j divides n */

    /* count primes strictly between j and k */
    seq_t count = 0;
    for (idx_t p = j + 1; p < k; ++p)
    {
        if (isPrimeTrial((seq_t)p))
            ++count;
    }

    return count;
}

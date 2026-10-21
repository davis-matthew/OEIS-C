/*
 * A093032  Smallest k>0 such that at least two primes exist that are not less
 *          than n-k and not greater than n+k.
 * https://oeis.org/A093032
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest k > 0 such that there are at least two primes *
 *            in the interval [ n‑k , n+k ]. The function tests primes on the
 *            fly * using the provided `is_prime(idx_t)` routine and stops as
 *            soon as * two distinct primes are found
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include <stdbool.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Count whether n itself is prime (it should be counted only once). */
    seq_t count = is_prime(n) ? 1 : 0;

    for (idx_t d = 1; ; ++d)
    {
        /* Left side: n‑d, only if n ≥ d to avoid underflow. */
        if (n >= d)
        {
            idx_t left = n - d;
            if (left >= 2 && is_prime(left))
                ++count;
        }

        /* Right side: n + d.  Check for unsigned overflow. */
        idx_t right = n + d;
        if (right < n)          /* overflow detected */
            break;
        if (right >= 2 && is_prime(right))
            ++count;

        /* If we have at least two primes, d is the desired k. */
        if (count >= 2)
            return (seq_t)d;
    }

    /* Should not be reached for ordinary inputs. */
    return 0;
}

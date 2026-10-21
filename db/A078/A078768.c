/*
 * A078768  a(n) is the number of iterations of k -> sigma(k) + 1 needed to
 *          reach a prime, or 0 if no prime is reached.
 * https://oeis.org/A078768
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   is_power_of_two, is_prime_big, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A078768
   a(n) = number of iterations of k -> sigma(k)+1 needed to reach a prime,
   or 0 if no prime is reached.  */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sigma_of.h"
#include "../utils/is_prime_big.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    /* If the start is a power of two greater than 2 the sequence never
       reaches a prime because sigma(2^k)+1 = 2^(k+1). */
    if (is_power_of_two((seq_t)n) && n > 2)
        return (seq_t)0;

    seq_t cur   = (seq_t)n;
    seq_t steps = 0;

    for (;;)
    {
        /* cur must fit in idx_t to call sigma_of */
        if (cur > (idx_t)-1)
            return (seq_t)0;

        idx_t idx = (idx_t)cur;
        seq_t next = sigma_of(idx) + 1;
        ++steps;

        if (is_prime_big(next))
            return steps;

        if (is_power_of_two(next) && next > 2)
            return (seq_t)0;

        cur = next;

        /* safety watchdog – the known sequences terminate far before this
           limit for all n ≤ 10000. */
        if (steps > 5000)
            return (seq_t)0;
    }
}

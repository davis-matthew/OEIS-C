/*
 * A174090  Powers of 2 and odd primes; alternatively, numbers that cannot be
 *          written as a sum of at least three consecutive positive integers.
 * https://oeis.org/A174090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 6557, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_power_of_two, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = nth member of {1} ∪ {odd primes} ∪ {powers of two} (OEIS A174090) */

#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_power_of_two.h"

seq_t a(idx_t n)
{
    /* Handle empty or invalid index request */
    if (n == 0) return 0;

    idx_t count = 0;
    for (idx_t i = 1; ; ++i)
    {
        /* A member if it is 1, a power of two, or an odd prime */
        bool good = (i == 1) ||
                    is_power_of_two((seq_t)i) ||
                    ((i & 1) && is_prime_small((seq_t)i));

        if (good)
        {
            ++count;
            if (count == n)
                return (seq_t)i;
        }
    }
}

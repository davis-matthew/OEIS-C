/*
 * A193742  Prime numbers ending in Hardy-Ramanujan number 1729.
 * https://oeis.org/A193742
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the n‑th prime of the form 10000 k + 1729
 * Range:     u8 none, u16 n <= 2, u32 n <= 3, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* It is computed by checking successive candidates until the desired
   count is reached, using the utility is_prime_big() for primality. */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* undefined: OEIS offset starts at 1 */

    idx_t count = 0;             /* number of primes found so far */
    idx_t k = 0;                 /* current value of k in 10000 k + 1729 */
    seq_t candidate, result = 0;

    /* Find the n‑th prime of the required form. */
    while (count < n)
    {
        candidate = (seq_t)10000 * (seq_t)k + (seq_t)1729;

        if (is_prime_big(candidate))
        {
            ++count;
            if (count == n)
            {
                result = candidate;
                break;
            }
        }
        ++k;
    }
    return result;
}

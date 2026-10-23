/*
 * A176165  Primes p such that (p-2)/11 is not a prime number.
 * https://oeis.org/A176165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime p such that (p-2)/11 is not a prime number
 * Range:     u8 n <= 52, u16 n <= 6420, u32 n <= 9800, u64 n <= 9800,
 *            u128 n <= 9800, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"   /* provides bool is_prime(idx_t n); */

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for indices < 1 */

    idx_t count = 0;              /* how many terms found */
    idx_t candidate = 2;          /* next integer to test */

    while (count < n)
    {
        /* find next prime */
        while (!is_prime(candidate))
            ++candidate;

        /* accept prime unless (p-2)/11 is an integer and prime */
        bool accept = true;
        if ((candidate - 2) % 11 == 0)
        {
            idx_t q = (candidate - 2) / 11;
            if (is_prime(q))
                accept = false;
        }

        if (accept)
        {
            ++count;
            if (count == n)
                return (seq_t)candidate;   /* cast to seq_t (handles built‑in or big integer) */
        }

        ++candidate;
    }

    return 0;   /* unreachable, but keeps function coverable */
}

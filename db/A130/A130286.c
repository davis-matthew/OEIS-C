/*
 * A130286  Strongly-single primes: primes p such that neither
 *          previousprime(p) nor nextprime(p) is a twin prime.
 * https://oeis.org/A130286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th strongly‑single prime, * i.e. a prime p for
 *            which neither previousprime(p) * nor nextprime(p) is a twin
 *            prime
 * Range:     u8 n <= 5, u16 n <= 3446, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/next_prime.h"

static bool is_twin_prime(uint64_t p)
{
    if (p < 3) return false;
    return is_prime(p - 2) || is_prime(p + 2);
}

seq_t a(idx_t n)
{
    /* Find primes in order and test each for the strongly‑single property. */
    uint64_t prevprev = next_prime(1ULL);          /* first prime 2   */
    uint64_t prev     = next_prime(prevprev);     /* second prime 3  */
    uint64_t curr     = next_prime(prev);         /* third prime 5   */
    seq_t   count    = 0;                          /* number found so far */

    for (;;)
    {
        /* Evaluate whether 'prev' satisfies the property. */
        bool twin_neighbor = false;

        if (prevprev != 0 && is_twin_prime(prevprev))
            twin_neighbor = true;
        if (is_twin_prime(curr))
            twin_neighbor = true;

        if (!twin_neighbor)
        {
            count++;
            if (count == n)
                return (seq_t)prev;      /* cast to seq_t is safe when it fits */
        }

        /* Advance the sliding window of primes. */
        prevprev = prev;
        prev     = curr;
        curr     = next_prime(curr);
    }
}

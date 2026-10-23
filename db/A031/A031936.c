/*
 * A031936  Lower prime of a difference of 18 between consecutive primes.
 * https://oeis.org/A031936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 358, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = lower prime of a pair of consecutive primes spaced by 18.
   Search primes sequentially with next_prime until the nth such pair is found. */

#include <stdint.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;                /* undefined below offset – safe sentinel */

    idx_t  count = 0;                    /* number of gaps 18 found so far */
    uint64_t p = 2;                      /* current prime */

    for (;;) {
        uint64_t q = next_prime(p + 1);  /* next prime after p */

        if (q - p == 18ULL) {
            ++count;
            if (count == n)
                return (seq_t)p;        /* found the required lower prime */
        }

        p = q;                          /* advance to the next prime */
    }
}

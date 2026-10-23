/*
 * A045326  Primes congruent to {2, 3} mod 4.
 * https://oeis.org/A045326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3285, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n): the n‑th prime congruent to 2 or 3 modulo 4.
   Uses a sieve up to a bound that guarantees enough primes.
*/
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    if (n == 1u) {
        return (seq_t)2u;           /* a(1) = 2 */
    }

    /* We look for the (n‑1)th prime that is 3 mod 4, because 2 is the first term. */
    idx_t target = n - 1u;
    idx_t limit  = n * 25u + 100u;   /* initial bound, generous for n up to 10 000 */

    while (true) {
        uint64_t *primes = NULL;
        size_t primes_cnt = 0;

        /* Build primes up to 'limit'.  The helper allocates an array we must free. */
        build_primes_upto(limit, &primes, &primes_cnt);

        idx_t count = 0;
        for (size_t i = 0; i < primes_cnt; ++i) {
            uint64_t p = primes[i];
            if (p == 2u) continue;            /* skip the even prime for subsequent terms */
            if ((p & 3u) == 3u) {              /* p % 4 == 3 */
                ++count;
                if (count == target) {
                    seq_t result = (seq_t)p;
                    free(primes);
                    return result;
                }
            }
        }
        free(primes);                         /* not found: increase bound and retry */
        limit <<= 1;                          /* double the limit */
    }
}

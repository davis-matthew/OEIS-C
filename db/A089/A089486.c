/*
 * A089486  The second-smallest prime of the form (p-prime(n))/(prime(n)-1),
 *          where p is also prime.
 * https://oeis.org/A089486
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the second smallest prime r such that prime(n) +
 *            (prime(n) - 1) * r is prime
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, isPrime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/isPrime.h"

seq_t a(idx_t n)
{
    /* obtain the nth prime */
    seq_t q_seq = get_prime(n);
    uint64_t q = (uint64_t)q_seq;            /* fits in 64 bits for n ≤ 10⁴ */
    uint64_t d = q - 1;                      /* denominator in the ratio */

    uint64_t count = 0;
    seq_t found = 0;                          /* will hold the second r */

    /* iterate over all prime r (starting from 2) */
    for (uint64_t r = 2; ; ++r) {
        if (!isPrime(r)) continue;           /* skip non‑primes */

        /* p = q + d * r */
        unsigned __int128 prod = (unsigned __int128)d * r;
        uint64_t p = q + (uint64_t)prod;     /* product fits in 64 bits */

        if (isPrime(p)) {
            ++count;
            if (count == 2) {                /* second prime r found */
                found = (seq_t)r;
                break;
            }
        }
    }
    return found;
}

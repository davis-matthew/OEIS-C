/*
 * A189172  Largest prime number tried when factoring n using trial division.
 * https://oeis.org/A189172
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = largest prime tried when factoring n using trial
 *            division. * * For each n ≥ 1, factor n by trial division with
 *            the primes in increasing * order. After factorising the current
 *            prime p fully, if the remaining part * r satisfies r ≤ p² the
 *            factorisation can stop – r is then prime. The * last...
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Handle the trivial cases n = 1, 2, 3 directly – they all return 1. */
    if (n <= 3) {
        return (seq_t)1;
    }

    uint64_t r = (uint64_t)n;   /* remaining part of the factorisation   */
    uint64_t last = 1;          /* last prime that was tried so far     */

    for (uint64_t p = 2; p <= r / p; ++p) {
        if (!is_prime(p)) {
            continue;                  /* skip composite candidates          */
        }

        last = p;                       /* p has now been tried               */

        /* Divide out all factors of p. */
        while (r % p == 0) {
            r /= p;
        }

        /* If the remaining part r is already at most p²,
           r is prime and we can stop. */
        if (p > r / p) {                /* p*p > r equivalently               */
            break;
        }
    }

    return (seq_t)last;
}

/*
 * A085416  Take prime(n) and continue adding 1,2,..., A085415(n) until one
 *          reaches a prime a(n).
 * https://oeis.org/A085416
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 40, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A085416: for the nth prime pₙ, it
   searches for the smallest k≥1 such that pₙ + 1+2+…+k is prime
   and returns that prime.  The search uses an incremental
   triangular‑sum accumulator.  A global cache of primes is kept
   between calls. */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "../utils/build_primes.h"   /* build_primes */
#include "../utils/is_prime_big.h"   /* is_prime_big */

seq_t a(idx_t n)
{
    /* Cache of primes – 1‑based position (primes[0] = 2). */
    static idx_t *primes = NULL;
    static size_t prime_cnt = 0;    /* number of primes cached */
    static idx_t prime_limit = 0;   /* current sieve limit */

    /* Ensure we have at least the nth prime.  Grow the sieve limit
       exponentially until enough primes are obtained. */
    while (prime_cnt < n) {
        if (prime_limit == 0)
            prime_limit = 100;          /* start with a small limit */
        else
            prime_limit *= 2;           /* double until enough primes */
        build_primes(prime_limit, &primes, &prime_cnt);
    }

    /* nth prime (primes array is 0‑based). */
    seq_t p = (seq_t)primes[n - 1];

    /* Find the first k such that p + triangular number is prime. */
    seq_t sum = 0;   /* 0 + 1 + 2 + ... + (k-1) accumulated incrementally */
    for (idx_t k = 1; ; ++k) {
        sum += (seq_t)k;                      /* add k to the triangular sum */
        seq_t candidate = p + sum;            /* p + T_k */
        if (is_prime_big(candidate))
            return candidate;
    }
}

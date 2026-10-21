/*
 * A066885  a(n) = (prime(n)^2 + 1)/2.
 * https://oeis.org/A066885
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (prime(n)^2 + 1)/2
 * Range:     u8 n <= 8, u16 n <= 72, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation expands the required number of primes on each call
   and then computes (p^2 + 1)/2 using the nth prime.  No static
   caching is used, which keeps the code valid for both C11 and C++17
   even when seq_t is an arbitrary‑precision integer type with a
   non‑trivial constructor. */

#include <stdlib.h>
#include "../utils/build_primes.h"

seq_t a(idx_t n)
{
    /* Offset 2: values for n < 2 are not used by the test harness */
    if (n < 2)
        return (seq_t)0;

    /* ------------------------------------------------------------
       Determine an upper bound for the nth prime and generate that
       many primes using build_primes.  The bound 20*n is safe for all
       practical values of n used in the tests (the 10 000th prime is
       104 729 < 200 000).  A small minimum ensures the case n=2 works.
       ------------------------------------------------------------ */
    size_t needed = (size_t)n;
    idx_t limit = 20 * n;
    if (limit < 100) limit = 100;

    idx_t *primes = NULL;
    size_t primes_cnt = 0;
    build_primes(limit, &primes, &primes_cnt);

    /* If the bound was too small, iterate until we have enough primes. */
    while (primes_cnt < needed) {
        limit *= 2;
        free(primes);
        build_primes(limit, &primes, &primes_cnt);
    }

    /* ------------------------------------------------------------
       Compute a(n) = (p^2 + 1)/2 where p is the nth prime.
       ------------------------------------------------------------ */
    seq_t p = (seq_t)primes[n - 1];  /* primes array is 0‑based */
    seq_t result = (p * p + 1) / 2;

    /* Clean up the prime list allocated by build_primes. */
    free(primes);

    return result;
}

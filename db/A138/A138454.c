/*
 * A138454  a(n) = ((n-th prime)^6-(n-th prime)^4)/3.
 * https://oeis.org/A138454
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p(n))^6 – (p(n))^4)/3
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 15, u64 n <= 297, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 980 (2026-11-01)
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  where p(n) is the n‑th prime (offset 1)          */
/*  Implements memoized prime generation using the
    provided utilities, and uses 128‑bit arithmetic
    to avoid overflow for the intermediate
    computations.                               */

#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static size_t _prime_limit_for_idx(idx_t n) {
    /* Rough upper bound for the n-th prime using n*(log n + log log n) + 3 */
    if (n < 6) return 15; /* first 6 primes < 15 */
    double nn = (double)n;
    double bound = nn * (log(nn) + log(log(nn))) + 3.0;
    return (size_t)bound;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  */
    if (n < 1)
        return 0;

    /* Static cache of primes */
    static uint64_t *primes = NULL;   /* array of primes */
    static size_t prime_cnt = 0;      /* number of primes cached */
    static idx_t sieve_limit = 0;     /* current sieve bound */

    /* Extend the cache until it contains at least n primes */
    while (prime_cnt < (size_t)n)
    {
        idx_t limit;
        if (sieve_limit == 0)
            /* Initial bound: a safe upper estimate for the n‑th prime,
               plus a small margin. */
            limit = _prime_limit_for_idx(n) + 10;
        else
            /* Double the bound to guarantee more primes */
            limit = sieve_limit * 2;

        /* Allocate a new prime array and build primes up to the new limit */
        uint64_t *new_primes = NULL;
        size_t new_cnt = 0;
        build_primes_upto(limit, &new_primes, &new_cnt);

        /* Replace the old cache */
        if (primes)
            free(primes);
        primes    = new_primes;
        prime_cnt = new_cnt;
        sieve_limit = limit;
    }

    /* The n‑th prime (1‑based index) is in primes[n‑1] */
    uint64_t p = primes[n - 1];

    /* Compute ((p^6 - p^4)/3) using 128‑bit arithmetic */
    __uint128_t p128 = ( __uint128_t ) p;
    __uint128_t p2   = p128 * p128;
    __uint128_t p4   = p2 * p2;
    __uint128_t p6   = p4 * p2;
    __uint128_t res  = (p6 - p4) / 3;

    return (seq_t)res;
}

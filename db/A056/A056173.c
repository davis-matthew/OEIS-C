/*
 * A056173  Number of unitary prime divisors of central binomial coefficient
 *          C(n, floor(n/2)) (A001405).
 * https://oeis.org/A056173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of unitary prime divisors of C(n, floor(n/2))
 * Range:     u8 n <= 2622, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

static uint64_t *primes   = NULL;   /* prime list (uint64_t) */
static size_t   primes_cnt = 0;     /* number of primes stored */
static idx_t    primes_limit = 0;   /* highest n for which primes were generated */

/* Generate all primes up to 'limit', freeing any previous list. */
static void ensure_primes_upto(idx_t limit)
{
    if (limit <= primes_limit) return;            /* already good */

    /* free old prime list, if any */
    free(primes);
    primes = NULL;
    primes_cnt = 0;
    primes_limit = 0;

    /* Util function 'build_primes_upto' allocates the array. */
    build_primes_upto(limit, &primes, &primes_cnt);
    primes_limit = limit;
}

/* v_p(n!) = exponent of prime p in n! */
static idx_t vp_factorial(idx_t n, idx_t p)
{
    idx_t exp = 0;
    idx_t power = p;
    while (power <= n) {
        exp += n / power;                     /* integer division */
        if (power > n / p) break;             /* next multiplication would overflow */
        power *= p;
    }
    return exp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* safety – offset starts at 1 */

    idx_t k   = n / 2;                   /* floor(n/2) */
    idx_t nk  = n - k;                   /* n-k = ceil(n/2) */

    /* Ensure we have all primes up to n */
    ensure_primes_upto(n);

    idx_t count = 0;

    for (size_t i = 0; i < primes_cnt; ++i) {
        idx_t p = primes[i];
        if (p > n) break;                /* primes larger than n do not divide C(n,k) */

        idx_t exp = vp_factorial(n,  p)
                  - vp_factorial(k,  p)
                  - vp_factorial(nk, p);

        if (exp == 1) ++count;           /* unitary prime divisor */
    }

    return (seq_t)count;
}

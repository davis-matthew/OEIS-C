/*
 * A048784  a(n) = tau(binomial(2*n,n)), where tau = number of divisors
 *          (A000005).
 * https://oeis.org/A048784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = τ( binomial(2*n, n) ) where τ(x) = number of divisors of
 *            x
 * Range:     u8 n <= 13, u16 n <= 37, u32 n <= 88, u64 n <= 226,
 *            u128 n <= 541, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation computes prime exponents of the central
 * binomial coefficient using Legendre's formula and multiplies
 * the contributions (e+1) for each prime p.  A simple
 * dynamic sieve generates all primes up to 2*n the first time
 * a larger n is requested.  The routine works for any
 * integer type seq_t provided by the build system and does
 * not use floating point or external libraries. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

static uint64_t *prime_tbl   = NULL; /* array of primes */
static size_t   prime_cnt    = 0;    /* number of primes in the array    */
static idx_t    prime_limit  = 0;    /* maximum number for which primes
                                        have been precomputed           */

 /* Simple sieve of Eratosthenes that fills prime_tbl and prime_cnt
  * for all primes ≤ limit.  The array is reallocated if necessary. */
static void build_primes_upto_limit(idx_t limit)
{
    if (limit <= prime_limit) return;          /* already have enough */

    /* allocate temporary boolean array */
    size_t sz = (size_t)limit + 1;
    bool *is_composite = (bool*)malloc(sz);
    if (!is_composite) { exit(1); }

    for (size_t i = 0; i < sz; ++i) is_composite[i] = false;
    for (size_t p = 2; p * p <= limit; ++p)
        if (!is_composite[p])
            for (size_t m = p * p; m <= limit; m += p)
                is_composite[m] = true;

    /* First count primes */
    size_t count = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) ++count;

    /* Reallocate prime_tbl if needed */
    if (count > prime_cnt) {
        uint64_t *tmp = (uint64_t*)realloc(prime_tbl, count * sizeof(uint64_t));
        if (!tmp) { free(is_composite); exit(1); }
        prime_tbl = tmp;
        prime_cnt = count;
    }

    /* Store primes */
    size_t idx = 0;
    for (size_t i = 2; i <= limit; ++i)
        if (!is_composite[i]) prime_tbl[idx++] = (uint64_t)i;

    free(is_composite);
    prime_limit = limit;
}

seq_t a(idx_t n)
{
    /* 2*n might overflow for n > UINT64_MAX/2 but this is
       beyond any practical test case. */
    idx_t limit = n * 2;

    /* Ensure we have primes up to 2*n */
    build_primes_upto_limit(limit);

    /* product of (exponent+1) for each prime */
    seq_t tau = (seq_t)1;

    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_tbl[i];
        if (p > (uint64_t)limit) break;

        idx_t e = 0;
        idx_t power = p;
        while (power <= limit) {
            e += (limit / power) - 2 * (n / power);
            if (power > limit / p) break;          /* avoid overflow */
            power *= p;
        }
        tau = tau * (seq_t)(e + 1);
    }
    return tau;
}

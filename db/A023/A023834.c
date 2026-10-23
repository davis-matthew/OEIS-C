/*
 * A023834  Sum of exponents in prime-power factorization of C(4n,2n).
 * https://oeis.org/A023834
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∑_{p ≤ 4n} (v_p((4n)!)-2·v_p((2n)!)) * where v_p(k!) =
 *            ⎣k/p⎦ + ⎣k/p²⎦ + … * * The function uses the provided
 *            build_primes() utility to obtain all * primes up to the current
 *            maximum 4n needed, then evaluates the * Legendre exponents for
 *            4n! and 2n! and returns the difference
 * Range:     u8 n <= 596, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/build_primes.h"

static idx_t *prime_list = NULL;      /* array of primes */
static size_t prime_count  = 0;       /* number of primes */
static idx_t  prime_limit  = 0;       /* highest prime generated so far */

/* Ensure we have all primes ≤ limit.  The global array is reallocated
 * each time a larger limit is required.  build_primes() allocates the
 * array with malloc, so it can safely be freed before the next call. */
static void
ensure_primes(idx_t limit)
{
    if (limit <= prime_limit)
        return;

    if (prime_list != NULL)
        free(prime_list);

    if (limit < 2) {          /* no primes needed */
        prime_list = NULL;
        prime_count = 0;
        prime_limit = limit;
        return;
    }

    build_primes(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Legendre exponent of prime p in m!  (m is small, ≤ 4n). */
static seq_t
exp_factorial(idx_t m, idx_t p)
{
    seq_t res = 0;
    idx_t t = m / p;
    while (t > 0) {
        res += t;
        t /= p;
    }
    return res;
}

/* Main sequence function */
seq_t
a(idx_t n)
{
    idx_t m4 = 4 * n;   /* 4n */
    idx_t m2 = 2 * n;   /* 2n */

    if (m4 < 2)        /* n == 0 case */
        return 0;

    ensure_primes(m4);

    seq_t sum4 = 0;
    seq_t sum2 = 0;

    for (size_t i = 0; i < prime_count && prime_list[i] <= m4; ++i) {
        idx_t p = prime_list[i];
        sum4 += exp_factorial(m4, p);
        sum2 += exp_factorial(m2, p);
    }

    return sum4 - (seq_t)2 * sum2;
}

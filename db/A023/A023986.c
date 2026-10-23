/*
 * A023986  Sum of exponents of primes in C(4n,2n) - sum of exponents of
 *          primes in C(2n,n).
 * https://oeis.org/A023986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = v_4n! - 3*v_2n! + 2*v_n! (sum of prime exponents * in
 *            C(4n,2n) minus that in C(2n,n))
 * Range:     u8 n <= 1559, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>      /* free, malloc */
#include <stdbool.h>     /* for bool if needed */
#include "../utils/build_primes.h"

/* Legendre’s formula: exponent of prime p in k! */
static unsigned long long v_factorial(idx_t k, idx_t p)
{
    unsigned long long exp = 0;
    idx_t power = p;
    while (power <= k) {
        exp += k / power;
        if (power > k / p) break;          /* avoid overflow         */
        power *= p;
    }
    return exp;
}

/* Cached prime list */
static idx_t *prime_list = NULL;
static size_t prime_count = 0;
static idx_t prime_limit = 0;

/* Rebuild prime list up to 'limit' if necessary */
static void ensure_primes(idx_t limit)
{
    if (limit <= prime_limit) return;
    if (prime_list) free(prime_list);
    build_primes(limit, &prime_list, &prime_count);
    prime_limit = limit;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;

    idx_t limit = 4 * n;
    idx_t two_n = 2 * n;

    ensure_primes(limit);

    unsigned long long sum = 0;

    for (size_t i = 0; i < prime_count && prime_list[i] <= limit; ++i) {
        idx_t p = prime_list[i];
        unsigned long long e4  = v_factorial(limit, p);
        unsigned long long e2  = v_factorial(two_n, p);
        unsigned long long en  = v_factorial(n, p);

        sum += e4 - 3 * e2 + 2 * en;
    }

    return (seq_t)sum;
}

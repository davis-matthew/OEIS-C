/*
 * A023837  Sum of exponents in prime-power factorization of C(5n,n).
 * https://oeis.org/A023837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A001222(A001449(n)) = sum of exponents in the
 *            prime‐factorisation * of the binomial coefficient C(5n,n)
 * Range:     u8 n <= 678, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Requires the helper that generates a list of primes up to a limit. */
#include "../utils/build_primes.h"
#include <stdlib.h>
#include <stddef.h>

/* Local helper: Legendre's formula for the exponent of prime p in L!. */
static seq_t exp_factorial(idx_t L, idx_t p)
{
    idx_t div = L / p;
    seq_t exp = 0;
    while (div)
    {
        exp += (seq_t)div;
        div /= p;
    }
    return exp;
}

seq_t a(idx_t n)
{
    /* C(0,0) = 1, no prime factors. */
    if (n == 0)
        return 0;

    /* Limit for the primes we need: up to 5n. */
    idx_t limit = 5 * n;

    /* Static cache of primes up to the largest limit requested so far. */
    static idx_t *prime_list = NULL;
    static size_t prime_count = 0;
    static idx_t current_limit = 0;

    if (limit > current_limit)
    {
        free(prime_list);
        build_primes(limit, &prime_list, &prime_count);
        current_limit = limit;
    }

    seq_t total = 0;

    /* Sum over all primes p <= 5n:
     * exponent in C(5n,n) = v_p((5n)!) – v_p(n!) – v_p((4n)!). */
    for (size_t i = 0; i < prime_count; ++i)
    {
        idx_t p = prime_list[i];
        seq_t e5 = exp_factorial(limit, p);
        seq_t e1 = exp_factorial(n, p);
        seq_t e2 = exp_factorial(4 * n, p);
        total += (e5 - e1 - e2);
    }

    return total;
}

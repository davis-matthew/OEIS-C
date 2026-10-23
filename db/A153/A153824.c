/*
 * A153824  Sum of proper divisors of n!: a(n) = sigma(n!) - n!.
 * https://oeis.org/A153824
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sigma(n!) – n! (sum of proper divisors of n!)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 33,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/build_primes_upto.h"

seq_t a(idx_t n)
{
    /* Prime list cache (primes up to the largest n seen so far). */
    static idx_t *prime_list = NULL;   /* array of primes */
    static size_t   prime_cnt  = 0;    /* number of primes stored   */
    static idx_t    primes_limit = 0;  /* upper bound for current list */

    /* If we need primes beyond what we already have, rebuild the list. */
    if (n > primes_limit)
    {
        /* Free any previous allocation (acceptable since this happens rarely). */
        if (prime_list)
            free(prime_list);

        /* build_primes_upto returns an array of primes ≤ n. */
        build_primes_upto(n, (uint64_t **)&prime_list, &prime_cnt);
        primes_limit = n;
    }

    /* 0! = 1 and 1! = 1 give a(0) = a(1) = 0. */
    if (n <= 1)
        return 0;

    /* Compute n! in arbitrary‑precision type seq_t. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Compute σ(n!) via prime factorization of n!. */
    seq_t sigma = 1;
    for (size_t pi = 0; pi < prime_cnt; ++pi)
    {
        idx_t p = prime_list[pi];
        if (p > n)
            break;

        /* Legendre’s formula for exponent of p in n!. */
        idx_t e = 0, div = p;
        while (div <= n)
        {
            e += n / div;
            div *= p;
        }

        /* Sum of powers: 1 + p + p² + … + pᵉ. */
        seq_t sum  = 1;    /* p^0 */
        seq_t pow  = 1;    /* p^0 */
        for (idx_t i = 1; i <= e; ++i)
        {
            pow *= (seq_t)p;   /* next power */
            sum += pow;        /* accumulate */
        }
        sigma *= sum;
    }

    /* Return sum of proper divisors: σ(n!) – n!. */
    return sigma - fact;
}

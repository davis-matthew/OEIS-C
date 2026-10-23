/*
 * A048673  Permutation of natural numbers: a(n) = (A003961(n)+1) / 2 [where
 *          A003961(n) shifts the prime factorization of n one step towards
 *          larger primes].
 * https://oeis.org/A048673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A003961(n)+1)/2, where A003961 shifts the prime
 *            factorization of n to the next primes
 * Range:     u8 n <= 47, u16 n <= 2047, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"
#include "../utils/next_prime.h"

/* Computes the nth term of OEIS A048673.
   n >= 1.  The function works for any build of seq_t and idx_t. */
seq_t a(idx_t n)
{
    if (n == 1) /* empty factorization -> product 1 */
        return (seq_t)1;

    /* Temporary arrays for prime factors and exponents.
       64 entries is enough for n < 2^64. */
    uint64_t primes[64];
    uint64_t exps  [64];
    size_t   cnt   = 0;

    /* Factor n; cnt will contain the number of distinct prime factors. */
    factor_u64((uint64_t)n, primes, exps, &cnt);

    seq_t prod = (seq_t)1;

    /* For each prime factor p^e replace p by its successor prime. */
    for (size_t i = 0; i < cnt; ++i) {
        uint64_t p  = primes[i];
        uint64_t e  = exps[i];
        uint64_t np = next_prime(p);      /* successor prime of p */

        /* Multiply prod by np^e.  e is at most 63 for 64‑bit n. */
        for (uint64_t k = 0; k < e; ++k)
            prod = prod * (seq_t)np;
    }

    /* Final formula: (product + 1) / 2.  Division truncates as required. */
    return (prod + (seq_t)1) / (seq_t)2;
}

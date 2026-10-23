/*
 * A182816  Number of values b in Z/nZ such that b^n = b.
 * https://oeis.org/A182816
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{p|n} (1 + gcd(n-1, p-1)), where the product runs
 *            over distinct primes p dividing n. For n = 1 a(1) = 1
 * Range:     u8 n <= 256, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factorize, gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation relies on the helper function factorize
   to obtain the distinct prime divisors of n, and the helper
   function gcd to compute the greatest common divisor. */

#include <stdint.h>
#include <stddef.h>     /* for size_t */
#include "../utils/factorize.h"
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the special case n == 1 */
    if (n == 1)
        return (seq_t)1;

    /* n fits in 64 bits, and all calculations below fit in seq_t
       because the result never exceeds n (see analysis). */
    uint64_t nn = (uint64_t)n;

    /* Arrays for prime factors and their exponents.
       A 64‑bit integer has at most 64 distinct prime factors,
       so this is a safe bound. */
    uint64_t primes[64];
    int exps[64];      /* exponents are not needed for the product */
    int pf_count;

    /* Obtain the distinct prime factors of nn */
    pf_count = factorize(nn, primes, exps);

    seq_t result = 1;
    seq_t n_minus1 = (seq_t)(nn - 1);

    for (int i = 0; i < pf_count; ++i)
    {
        uint64_t p = primes[i];
        seq_t g = gcd(n_minus1, (seq_t)(p - 1));
        seq_t term = g + 1;
        result *= term;
    }

    return result;
}

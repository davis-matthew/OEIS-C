/*
 * A234505  a(n) = 2*binomial(9*n+2,n)/(9*n+2).
 * https://oeis.org/A234505
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*binomial(9*n+2,n)/(9*n+2)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 16, u128 n <= 30,
 *            big any n
 * Helpers:   build_primes_upto
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/build_primes_upto.h"

static idx_t exp_factorial(idx_t n, uint64_t p)
{
    idx_t e = 0;
    while (n)
    {
        n /= p;
        e += n;
    }
    return e;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    idx_t N = 9 * n + 2;
    uint64_t *primes = NULL;
    size_t prime_count = 0;

    /* Build list of primes up to N. */
    build_primes_upto((idx_t)N, &primes, &prime_count);

    seq_t result = (seq_t)1;

    for (size_t _i = 0; _i < prime_count; ++_i)
    {
        uint64_t p = primes[_i];

        idx_t e_bin   = exp_factorial(N,       p)
                     - exp_factorial(n,       p)
                     - exp_factorial(N - n,   p);   /* exponent in binomial */

        /* exponent of p in integer N */
        idx_t e_den = 0;
        uint64_t tmp = N;
        while (tmp % p == 0)
        {
            ++e_den;
            tmp /= p;
        }

        idx_t inc_two = (p == 2) ? 1 : 0;

        idx_t final_exp = e_bin - e_den + inc_two;

        if (final_exp > 0)
        {
            for (idx_t j = 0; j < final_exp; ++j)
                result = result * (seq_t)p;
        }
    }

    free(primes);
    return result;
}

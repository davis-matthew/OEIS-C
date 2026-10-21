/*
 * A206295  (1/6)*A007188(n).
 * https://oeis.org/A206295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/6) * A007188(n) A007188(n) = 2 * ∏_{k=1}^{n}
 *            p_k^{C(n,k)} where p_k is the k-th odd prime (3,5,7,…) Thus a(n)
 *            = 3^{n-1} * ∏_{k=2}^{n} p_k^{C(n,k)} (C(n,1)=n) The
 *            implementation uses big integer arithmetic (seq_t). It builds
 *            the first n odd primes by a simple sieve, computes the
 *            binomial...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include "../utils/big_pow.h"   /* seq_t big_pow(seq_t base, seq_t exp); */

seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 0)
        return (seq_t)0;

    /* compute binomial coefficients C(n, k) for k = 0..n */
    size_t N = (size_t)n;              /* for array indexing */
    seq_t *binom = (seq_t*)malloc((N + 1) * sizeof(seq_t));
    if (!binom)
        return (seq_t)0;               /* fallback on allocation failure */

    binom[0] = (seq_t)1;
    for (size_t k = 1; k <= N; ++k)
    {
        /* binom[k] = binom[k-1] * (n - k + 1) / k; */
        seq_t numer = binom[k - 1] * (seq_t)(n - k + 1);
        seq_t denom = (seq_t)k;
        binom[k] = numer / denom;
    }

    /* Estimate upper bound for n-th odd prime using Rosser's theorem */
    size_t est_limit;
    if (n < 6)
        est_limit = 15;
    else
    {
        double dn = (double)n;
        double lb = dn * (log(dn) + log(log(dn)));
        est_limit = (size_t)(lb + 10.0);
    }

    /* Simple sieve to collect first n odd primes (exclude 2) */
    bool *is_prime = (bool*)malloc((est_limit + 1) * sizeof(bool));
    if (!is_prime)
    {
        free(binom);
        return (seq_t)0;
    }
    for (size_t i = 0; i <= est_limit; ++i)
        is_prime[i] = true;
    is_prime[0] = is_prime[1] = false;
    for (size_t i = 2; i * i <= est_limit; ++i)
    {
        if (is_prime[i])
            for (size_t j = i * i; j <= est_limit; j += i)
                is_prime[j] = false;
    }

    /* collect odd primes */
    uint64_t *odd_primes = (uint64_t*)malloc(n * sizeof(uint64_t));
    if (!odd_primes)
    {
        free(is_prime);
        free(binom);
        return (seq_t)0;
    }
    size_t cnt = 0;
    for (size_t i = 3; i <= est_limit && cnt < n; i += 2)
    {
        if (is_prime[i])
            odd_primes[cnt++] = i;
    }
    if (cnt < n)   /* fallback: extend sieve if needed */
    {
        size_t new_limit = est_limit * 2;
        bool *new_prime = (bool*)malloc((new_limit + 1) * sizeof(bool));
        if (!new_prime)
        {
            free(odd_primes);
            free(is_prime);
            free(binom);
            return (seq_t)0;
        }
        for (size_t i = 0; i <= new_limit; ++i)
            new_prime[i] = true;
        new_prime[0] = new_prime[1] = false;
        for (size_t i = 2; i * i <= new_limit; ++i)
            if (new_prime[i])
                for (size_t j = i * i; j <= new_limit; j += i)
                    new_prime[j] = false;
        for (size_t i = 3; i <= new_limit && cnt < n; i += 2)
        {
            if (new_prime[i])
                odd_primes[cnt++] = i;
        }
        free(new_prime);
    }

    free(is_prime);

    /* compute the product ∏ p_k^{exponent} */
    seq_t result = (seq_t)1;
    for (size_t i = 0; i < n; ++i)
    {
        seq_t exp;
        if (i == 0)                  /* prime 3 */
        {
            exp = binom[1] - (seq_t)1;   /* n - 1 */
        }
        else
        {
            exp = binom[i + 1];        /* C(n, i+1) */
        }
        seq_t pow_val = big_pow((seq_t)odd_primes[i], exp);
        result *= pow_val;
    }

    free(odd_primes);
    free(binom);
    return result;
}

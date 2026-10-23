/*
 * A023852  Sum of exponents in prime-power factorization of binomial(6n, n).
 * https://oeis.org/A023852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Ω(binomial(6n, n)) Equivalent: a(n) = Σ_{p ≤ 6n} (
 *            v_p((6n)!) – v_p(n!) – v_p((5n)!) ) where v_p is the exponent of
 *            p in N!
 * Range:     u8 n <= 621, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* static cache of primes up to the largest argument seen so far */
static idx_t *primes = NULL;
static size_t primes_cnt = 0;
static idx_t sieve_limit = 0;

/* Ensure that primes up to 'limit' are generated.  The sieve uses
   a classic Eratosthenes implementation.  All allocations are
   performed through calloc and cast to the appropriate type so
   that the code is valid in both C and C++. */
static void ensure_primes(idx_t limit)
{
    if (limit < 2)
    {
        /* nothing required */
        if (primes)
        {
            free(primes);
            primes = NULL;
        }
        primes_cnt = 0;
        sieve_limit = 0;
        return;
    }

    if (limit <= sieve_limit)
        return;                 /* already sufficient */

    /* free any previous array */
    if (primes)
    {
        free(primes);
        primes = NULL;
    }

    size_t size = (size_t)(limit + 1);
    bool *is_prime = (bool *)calloc(size, sizeof(bool));
    if (!is_prime)
    {
        primes_cnt = 0;
        sieve_limit = 0;
        return;
    }

    /* initialise */
    for (idx_t i = 2; i <= limit; ++i)
        is_prime[(size_t)i] = true;

    for (idx_t i = 2; (idx_t)i * i <= limit; ++i)
    {
        if (is_prime[(size_t)i])
        {
            for (idx_t j = i * i; j <= limit; j += i)
                is_prime[(size_t)j] = false;
        }
    }

    /* count primes */
    size_t cnt = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            ++cnt;

    /* store primes */
    idx_t *p_arr = (idx_t *)calloc(cnt, sizeof(idx_t));
    if (!p_arr)
    {
        free(is_prime);
        primes_cnt = 0;
        sieve_limit = 0;
        return;
    }

    size_t pos = 0;
    for (idx_t i = 2; i <= limit; ++i)
        if (is_prime[(size_t)i])
            p_arr[pos++] = i;

    primes = p_arr;
    primes_cnt = cnt;
    sieve_limit = limit;

    free(is_prime);
}

/* Compute Legendre exponent of prime p in N! */
static idx_t legendre(idx_t N, idx_t p)
{
    idx_t exp = 0;
    idx_t div = p;
    while (div <= N)
    {
        exp += N / div;
        if (div > N / p)
            break;
        div *= p;
    }
    return exp;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    idx_t m6 = 6 * n;  /* 6n */
    idx_t m5 = 5 * n;  /* 5n */

    ensure_primes(m6);  /* ensure prime list up to 6n */

    seq_t result = (seq_t)0;
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        idx_t p = primes[i];
        if (p > m6)
            break;
        idx_t e6 = legendre(m6, p);
        idx_t e1 = legendre(n, p);
        idx_t e5 = legendre(m5, p);
        result += (seq_t)(e6 - e1 - e5);
    }
    return result;
}

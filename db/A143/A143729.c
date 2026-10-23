/*
 * A143729  Sum of termwise product of mu(k) and reduced residue system k mod
 *          n.
 * https://oeis.org/A143729
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = Sum_{1<=k<=n, GCD(k,n)=1} mu(k)*k
 * Range:     i8 n <= 41, i16 n <= 989, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

/* Global sieve storage.  */
static idx_t      sieve_max   = 0;
static int8_t *   mu_arr      = NULL;
static uint8_t *  is_comp     = NULL;
static int *      primes      = NULL;
static size_t     primes_cnt  = 0;

/* Build the Möbius array up to n (inclusive) using a linear sieve.
   The static data is updated only if a larger n is requested. */
static void build_sieve(idx_t n)
{
    if (n <= sieve_max)
        return;

    /* Release old data. */
    free(mu_arr);
    free(is_comp);
    free(primes);

    /* Allocate new tables.  */
    mu_arr   = (int8_t *)malloc((n + 1) * sizeof(int8_t));
    is_comp  = (uint8_t *)malloc((n + 1) * sizeof(uint8_t));
    primes   = (int *)   malloc((n + 1) * sizeof(int));

    /* Clear the composite flag array.  */
    memset(is_comp, 0, (n + 1) * sizeof(uint8_t));

    mu_arr[0] = 0;
    mu_arr[1] = 1;
    size_t pi = 0;    /* count of primes seen */

    for (idx_t i = 2; i <= n; ++i)
    {
        if (!is_comp[i])
        {
            mu_arr[i] = -1;
            primes[pi++] = (int)i;
        }

        for (size_t j = 0; j < pi; ++j)
        {
            idx_t p = (idx_t)primes[j];
            idx_t x = i * p;
            if (x > n)
                break;
            is_comp[x] = 1;
            if (i % p == 0)
            {
                mu_arr[x] = 0;
                break;
            }
            else
                mu_arr[x] = -mu_arr[i];
        }
    }

    primes_cnt = pi;
    sieve_max  = n;
}

/* Compute a(n) as defined above.  */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, but guard anyway */
        return 0;

    /* Ensure μ(k) is known for 1 <= k <= n. */
    build_sieve(n);

    /* Find the distinct prime factors of n (used to test coprimality). */
    idx_t tmp = n;
    int bad[64];
    size_t bad_cnt = 0;

    for (size_t i = 0; i < primes_cnt &&
         (idx_t)primes[i] * (idx_t)primes[i] <= tmp; ++i)
    {
        int p = primes[i];
        if (tmp % p == 0)
        {
            bad[bad_cnt++] = p;
            while (tmp % p == 0)
                tmp /= p;
        }
    }
    if (tmp > 1)
        bad[bad_cnt++] = (int)tmp;    /* remaining prime factor */

    /* Sum μ(k)·k over k <= n with gcd(k,n)=1. */
    seq_t sum = 0;
    for (idx_t k = 1; k <= n; ++k)
    {
        int8_t mu_k = mu_arr[k];
        if (mu_k == 0)
            continue;

        bool coprime = true;
        for (size_t j = 0; j < bad_cnt; ++j)
        {
            if (k % bad[j] == 0)
            {
                coprime = false;
                break;
            }
        }
        if (!coprime)
            continue;

        sum += (seq_t)mu_k * (seq_t)k;
    }
    return sum;
}

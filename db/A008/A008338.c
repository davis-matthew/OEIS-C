/*
 * A008338  a(n+1) = a(n)/n! if n! divides a(n) else a(n)*n!.
 * https://oeis.org/A008338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 10, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A008338
   a(1) = 1
   For n >= 1: a(n+1) = a(n)/n! if n! divides a(n) else a(n)*n!  */

/* Utility functions and static tables */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* The types seq_t and idx_t are supplied by the build system. */

static size_t spf_size = 0;          /* length of the smallest‑prime‑factor array */
static uint32_t *spf = NULL;         /* SPF table */
static uint32_t *primes = NULL;      /* list of primes */
static size_t primes_cnt = 0;        /* number of primes stored */
static size_t *prime_idx = NULL;     /* mapping: prime → index in primes[] */

/* Ensure that spf[], primes[] and prime_idx[] contain data up to n. */
static void ensure_sieve(idx_t n)
{
    if (n < spf_size) return;          /* already sufficient */

    size_t new_len = (size_t)n + 1;    /* array indices go from 0 to n */

    uint32_t *new_spf = (uint32_t *)calloc(new_len, sizeof(uint32_t));
    uint32_t *new_primes = NULL;
    size_t new_primes_cnt = 0;
    size_t *new_prime_idx = (size_t *)calloc(new_len, sizeof(size_t));

    for (size_t i = 2; i < new_len; ++i)
    {
        if (new_spf[i] == 0)                /* i is prime */
        {
            new_spf[i] = (uint32_t)i;

            /* store the prime */
            new_primes = (uint32_t *)realloc(new_primes,
                                             (new_primes_cnt + 1) * sizeof(uint32_t));
            new_primes[new_primes_cnt] = (uint32_t)i;
            new_prime_idx[i] = new_primes_cnt;   /* map prime → index */

            ++new_primes_cnt;

            if (i * i < new_len)              /* cross‑out composites */
                for (size_t j = i * i; j < new_len; j += i)
                    if (new_spf[j] == 0)
                        new_spf[j] = (uint32_t)i;
        }
    }

    free(spf);
    free(primes);
    free(prime_idx);

    spf = new_spf;
    primes = new_primes;
    primes_cnt = new_primes_cnt;
    spf_size = new_len;
    prime_idx = new_prime_idx;
}

/* Integer exponentiation suitable for all seq_t types. */
static seq_t pow_uint(seq_t base, size_t exp)
{
    seq_t res = (seq_t)1;
    while (exp)
    {
        if (exp & 1U)
            res = res * base;
        base = base * base;
        exp >>= 1U;
    }
    return res;
}

/* -------------------------------------------------------------------- */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;                /* a(1) = 1 */

    /* Build sieve up to n to be able to factor all integers ≤ n. */
    ensure_sieve(n);

    size_t pcnt = primes_cnt;

    /* exponent arrays for a(n) and for the current factorial. */
    int *exp = (int *)calloc(pcnt, sizeof(int));
    int *fact_exp = (int *)calloc(pcnt, sizeof(int));

    /* Main loop: process k = 1 … n‑1 to obtain a(n). */
    for (idx_t k = 1; k < n; ++k)
    {
        /* decide divisible or not */
        bool divisible = true;
        for (size_t j = 0; j < pcnt; ++j)
            if (exp[j] < fact_exp[j]) { divisible = false; break; }

        if (divisible)         /* divide by k! */
        {
            for (size_t j = 0; j < pcnt; ++j)
                exp[j] -= fact_exp[j];
        }
        else                    /* multiply by k! */
        {
            for (size_t j = 0; j < pcnt; ++j)
                exp[j] += fact_exp[j];
        }

        /* Factorise (k+1) and add its exponents to fact_exp. */
        idx_t val = k + 1;
        while (val > 1)
        {
            uint32_t p = spf[val];
            size_t idx_p = prime_idx[p];      /* guaranteed to exist */
            int e = 0;
            while (val % p == 0)
            {
                val /= p;
                ++e;
            }
            fact_exp[idx_p] += e;
        }
    }

    /* Reconstruct the integer from its prime factors. */
    seq_t result = (seq_t)1;
    for (size_t j = 0; j < pcnt; ++j)
    {
        if (exp[j] > 0)               /* exp[j] is guaranteed non‑negative */
            result = result * pow_uint((seq_t)primes[j], (size_t)exp[j]);
    }

    free(exp);
    free(fact_exp);
    return result;
}

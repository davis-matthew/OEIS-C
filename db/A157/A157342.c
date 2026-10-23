/*
 * A157342  Semiprimes that are the product of two non-Sophie Germain primes.
 * https://oeis.org/A157342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 5000, u64 n <= 5000, u128 n <= 5000,
 *            big any n
 * Helpers:   build_primes_upto, compare_seq_t, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: semiprimes that are the product of two non‑Sophie Germain primes. */
/* Algorithm: generate all primes up to a growing limit, filter out the
   primes p for which 2p+1 is prime, then form all products p_i * p_j
   (i ≤ j), sort them and cache the results.  The function returns the
   n‑th smallest such product (a(1) is the first term). */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include "../utils/build_primes_upto.h"
#include "../utils/is_prime.h"
#include "../utils/compare_seq_t.h"

static seq_t *cache = NULL;      /* cached list of semiprimes */
static size_t cache_len = 0;     /* number of cached terms          */
static idx_t  prime_limit = 20000; /* current prime generation limit    */
static bool   initialized = false; /* flag indicating cache state */

/* ------------------------------------------------------------ */
/* Generate the sorted list of semiprimes until it contains at
   least 'need' elements.  The resulting array is kept in 'cache'.
   Reallocations are performed as necessary.                     */
static void recompute(idx_t need)
{
    /* loop until we have enough terms */
    while (true)
    {
        /* build primes up to the current limit */
        uint64_t *primes = NULL;
        size_t    prime_cnt = 0;
        build_primes_upto(prime_limit, &primes, &prime_cnt);

        /* collect primes that are NOT Sophie Germain */
        seq_t *nsg = NULL;
        size_t nsg_cnt = 0;
        if (prime_cnt > 0)
        {
            nsg = (seq_t *)malloc(prime_cnt * sizeof(seq_t));
            for (size_t i = 0; i < prime_cnt; ++i)
            {
                uint64_t p = primes[i];
                if (!is_prime((idx_t)(2 * p + 1)))   /* 2p+1 is composite */
                {
                    nsg[nsg_cnt++] = (seq_t)p;
                }
            }
        }

        /* number of semiprimes that can be formed from the list */
        size_t semis = nsg_cnt * (nsg_cnt + 1) / 2;
        if (semis == 0)
        {
            /* No non‑Sophie Germain primes: increase limit and retry. */
            free(nsg);
            free(primes);
            prime_limit *= 2;
            continue;
        }

        /* generate all semiprimes */
        seq_t *sem = (seq_t *)malloc(semis * sizeof(seq_t));
        size_t k = 0;
        for (size_t i = 0; i < nsg_cnt; ++i)
        {
            for (size_t j = i; j < nsg_cnt; ++j)
            {
                sem[k++] = nsg[i] * nsg[j];
            }
        }

        /* sort the list */
        qsort(sem, semis, sizeof(seq_t), compare_seq_t);

        /* check if we have enough terms */
        if (semis >= (size_t)need)
        {
            /* replace old cache */
            free(cache);
            cache     = sem;
            cache_len = semis;
            free(nsg);
            free(primes);
            return;
        }

        /* not enough – free memory and double the limit */
        free(sem);
        free(nsg);
        free(primes);
        prime_limit *= 2;
    }
}

/* ------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n <= 0, return 0 as placeholder */

    if (!initialized || (idx_t)cache_len < n)
    {
        recompute(n);
        initialized = true;
    }

    return cache[n - 1];
}

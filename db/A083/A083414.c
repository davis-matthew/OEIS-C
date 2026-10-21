/*
 * A083414  Write the numbers from 1 to n^2 consecutively in n rows of length
 *          n; let c(k) = number of primes in k-th column; a(n) = minimal c(k)
 *          for gcd(k,n) = 1.
 * https://oeis.org/A083414
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 839, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   gcd_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A083414:
 *  a(n) = min_{1≤k≤n, gcd(k,n)=1} (# of primes in the k-th column of the n×n array
 *  formed by writing 1,2,…,n² consecutively in rows).  The routine sieves all
 *  integers up to n² (growing a shared sieve across calls), counts the primes
 *  belonging to each residue class modulo n, then takes the minimum over
 *  coprime column indices.  All arithmetic uses only integral types; any
 *  overflow is avoided by counting primes incrementally.  The behaviour is
 *  deterministic and independent of call order.  The code is valid C11/C++17.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/gcd_int.h"

/*----------- static context for incremental sieve -----------*/
static unsigned char *isComposite = NULL;   /* 0 = prime, 1 = composite */
static int *primes = NULL;                 /* list of primes found */
static size_t primes_cap = 0;              /* capacity of primes array    */
static size_t primes_cnt = 0;              /* number of primes stored     */
static uint64_t sieve_limit = 0;           /* largest value currently sieved */

/*----- add a new prime to the list --------------------------------*/
static void add_prime(int p)
{
    if (primes_cnt == primes_cap)
    {
        size_t new_cap = primes_cap ? primes_cap << 1 : 16;
        int *tmp = (int *)realloc(primes, new_cap * sizeof(int));
        if (!tmp) return;      /* allocation failure – silently ignore */
        primes = tmp;
        primes_cap = new_cap;
    }
    primes[primes_cnt++] = p;
}

/*----- small gcd for loop indices ---------------------------------*/

/*----- extend the sieve up to 'limit' (inclusive) -----------------*/
static void ensure_sieve(uint64_t limit)
{
    if (limit <= sieve_limit) return;            /* already done */

    /* enlarge composite array */
    unsigned char *new_isComposite =
        (unsigned char *)realloc(isComposite, (size_t)(limit + 1) * sizeof(unsigned char));
    if (!new_isComposite) return;       /* allocation failure – abort */
    isComposite = new_isComposite;

    /* clear new part; positions 0 and 1 are deliberately left composite */
    if (sieve_limit + 1 <= limit)
    {
        memset(isComposite + sieve_limit + 1, 0,
               (size_t)(limit - sieve_limit) * sizeof(unsigned char));
    }

    /* mark composites in the new range using existing primes */
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        int p = primes[i];
        uint64_t start = (sieve_limit + p) / (uint64_t)p * (uint64_t)p;
        if (start < (uint64_t)p * 2) start = (uint64_t)p * 2;
        for (uint64_t j = start; j <= limit; j += (uint64_t)p)
            isComposite[(size_t)j] = 1;
    }

    /* sieve the newly added range (skipping 0 and 1) */
    for (uint64_t i = sieve_limit + 1; i <= limit; ++i)
    {
        if (i < 2) continue;          /* 0 and 1 are not primes */
        if (!isComposite[(size_t)i])
        {
            add_prime((int)i);
            if (i * (uint64_t)i <= limit)
            {
                for (uint64_t j = i * i; j <= limit; j += i)
                    isComposite[(size_t)j] = 1;
            }
        }
    }
    sieve_limit = limit;
}

/*-------------------- main sequence function --------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* invalid */
    uint64_t limit = (uint64_t)n * (uint64_t)n;
    ensure_sieve(limit);

    /* array to count primes per residue class modulo n */
    int *counts = (int *)calloc((size_t)n, sizeof(int));
    if (!counts) return 0;                    /* allocation failure */

    /* count primes in each column */
    for (size_t i = 0; i < primes_cnt; ++i)
    {
        int p = primes[i];
        if ((uint64_t)p > limit) break;
        int col = (p - 1) % (int)n;           /* zero‑based column index */
        ++counts[col];
    }

    /* find minimal count among columns coprime to n */
    int best = n;                              /* upper bound */
    for (int k = 1; k <= (int)n; ++k)
    {
        if (gcd_int(k, (int)n) == 1)
        {
            int val = counts[k - 1];
            if (val < best) best = val;
        }
    }

    free(counts);
    return (seq_t)best;
}

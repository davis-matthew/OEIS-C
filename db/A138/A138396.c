/*
 * A138396  Primes p such that the left prime neighbors p1, p2 of p as well as
 *          the right prime neighbors q1, q2 of p form twin prime pairs and
 *          the sum p1 + p2 + p + q1 + q2 is also prime.
 * https://oeis.org/A138396
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes A138396.  It sieves primes up to a
   dynamically increasing limit, collects the qualifying primes,
   and returns the n-th term.  It uses the helper
   bool is_prime_big(seq_t n) from the utilities for large sums. */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Guard against the offset: the first term is a(1). */
    if (n == 0) return 0;

    /* Track the largest n requested. */
    static idx_t max_requested = 0;
    if (n > max_requested) max_requested = n;

    /* Static state for the list of valid primes. */
    static seq_t *list = NULL;
    static size_t  list_len = 0;

    /* If we already have enough terms, return the answer. */
    if (n > 0 && n <= list_len) return list[n - 1];

    /* We have to compute more terms.  Maintain the sieve limit
       and recompute when we run out. */
    static size_t limit = 20000000U;          /* start with 20 million */
    static bool ready = false;

    /* Re‑compute until we have enough terms. */
    while (!ready || list_len < (size_t)max_requested)
    {
        /* 1. Sieve primes up to 'limit'. */
        uint8_t *isPrime = (uint8_t *)malloc((limit + 1) * sizeof(uint8_t));
        if (!isPrime) return 0;                 /* allocation failure */

        for (size_t i = 0; i <= limit; ++i) isPrime[i] = 1;
        isPrime[0] = isPrime[1] = 0;

        for (size_t p = 2; p * p <= limit; ++p)
            if (isPrime[p])
                for (size_t m = p * p; m <= limit; m += p)
                    isPrime[m] = 0;

        /* Count primes to allocate the array. */
        size_t primeCnt = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (isPrime[i]) ++primeCnt;

        uint32_t *primes = (uint32_t *)malloc(primeCnt * sizeof(uint32_t));
        if (!primes) { free(isPrime); return 0; }

        size_t pi = 0;
        for (size_t i = 2; i <= limit; ++i)
            if (isPrime[i]) primes[pi++] = (uint32_t)i;

        /* 2. Scan for qualifying primes. */
        seq_t *tmp_list = NULL;
        size_t  cap = 1024;
        tmp_list = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!tmp_list) { free(primes); free(isPrime); return 0; }

        size_t found = 0;
        for (size_t i = 2; i + 2 < primeCnt; ++i)
        {
            uint32_t p2 = primes[i - 2];
            uint32_t p1 = primes[i - 1];
            uint32_t  p = primes[i];
            uint32_t q1 = primes[i + 1];
            uint32_t q2 = primes[i + 2];

            if ((p1 - p2) != 2) continue;   /* left twin check   */
            if ((q2 - q1) != 2) continue;   /* right twin check  */

            seq_t sum = (seq_t)p2 + (seq_t)p1 + (seq_t)p
                      + (seq_t)q1 + (seq_t)q2;

            bool sumPrime;
            if (sum <= (seq_t)limit)
                sumPrime = !!isPrime[(size_t)sum];
            else
                sumPrime = is_prime_big(sum);

            if (sumPrime)
            {
                if (found == cap)
                {
                    cap <<= 1;
                    tmp_list = (seq_t *)realloc(tmp_list, cap * sizeof(seq_t));
                    if (!tmp_list) { free(primes); free(isPrime); return 0; }
                }
                tmp_list[found++] = (seq_t)p;
            }
        }

        /* 3. Store result, free temporary objects. */
        free(primes);
        free(isPrime);

        /* Shrink list to actual size. */
        seq_t *new_list = (seq_t *)realloc(tmp_list, found * sizeof(seq_t));
        if (!new_list && found > 0) { free(tmp_list); return 0; }
        /* Replace old list (if any) safely. */
        if (list) free(list);
        list     = new_list;
        list_len = found;
        ready    = true;

        /* If we still lack enough terms, double the limit. */
        if (list_len < (size_t)max_requested)
            limit <<= 1;
    }

    /* Return the requested term. */
    if (n > 0 && n <= list_len) return list[n - 1];
    return 0;   /* should not happen for valid inputs */
}

/*
 * A064899  Numbers that are of the form pq where p and q are distinct primes
 *          that are the orders of non-Abelian groups.
 * https://oeis.org/A064899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 1000, u64 n <= 1000, u128 n <= 1000,
 *            big any n
 * Helpers:   cmp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A064899.
   The sequence consists of all numbers of the form p*q where
   p and q are distinct primes, p < q, and p divides (q-1).
   (Equivalently: all semidirect product orders p⋊q of type pq
   with p < q.)  The function generates all such products,
   sorts and deduplicates them once and then returns the n‑th
   term (n ≥ 1).  The implementation uses only standard C/C++ features. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/cmp_seq.h"

/* The main sequence function */
seq_t a(idx_t n)
{
    /* static data: built only once */
    static bool initialized = false;
    static seq_t *products = NULL;   /* sorted list of pq */
    static size_t prod_cnt = 0;      /* number of products */

    if (!initialized)
    {
        initialized = true;

        /* 1. Generate all primes up to a fixed limit.
           A limit of 200000 guarantees more than 10000 terms
           (pi(200000)≈17984, each contributes ~log log q primes). */
        const uint32_t PRIME_LIMIT = 200000;

        bool *is_composite = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
        if (!is_composite) return 0;            /* safety */

        for (uint32_t i = 2; i * i <= PRIME_LIMIT; ++i)
            if (!is_composite[i])
                for (uint32_t j = i * i; j <= PRIME_LIMIT; j += i)
                    is_composite[j] = true;

        /* count primes */
        size_t prime_cnt = 0;
        for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
            if (!is_composite[i]) ++prime_cnt;

        /* collect primes */
        uint64_t *primes = (uint64_t *)malloc(prime_cnt * sizeof(uint64_t));
        if (!primes) { free(is_composite); return 0; }
        size_t pi = 0;
        for (uint32_t i = 2; i <= PRIME_LIMIT; ++i)
            if (!is_composite[i]) primes[pi++] = i;
        free(is_composite);

        /* 2. Generate all products p*q where p divides (q-1) and p < q. */
        size_t prod_cap = prime_cnt * 10;   /* initial capacity */
        products = (seq_t *)malloc(prod_cap * sizeof(seq_t));
        if (!products) { free(primes); return 0; }

        for (pi = 0; pi < prime_cnt; ++pi)          /* p = primes[pi] is smaller */
        {
            uint64_t p = primes[pi];
            for (size_t qi = pi + 1; qi < prime_cnt; ++qi)   /* q > p */
            {
                uint64_t q = primes[qi];
                if ((q - 1) % p == 0)
                {
                    if (prod_cnt >= prod_cap)
                    {
                        prod_cap *= 2;
                        products = (seq_t *)realloc(products,
                                                     prod_cap * sizeof(seq_t));
                        if (!products) { free(primes); return 0; }
                    }
                    products[prod_cnt++] = (seq_t)p * (seq_t)q;
                }
            }
        }

        free(primes);

        /* 3. Sort and remove duplicates (duplicates can occur if q has several
           prime factors p that both divide q-1). */
        qsort(products, prod_cnt, sizeof(seq_t), cmp_seq);
        size_t uniq = 0;
        for (size_t i = 0; i < prod_cnt; ++i)
        {
            if (i == 0 || products[i] != products[i - 1])
                products[uniq++] = products[i];
        }
        prod_cnt = uniq;
    }

    /* 4. Answer the query.  n is 1‑based. */
    if (n == 0 || (idx_t)(n - 1) >= prod_cnt)
        return 0;   /* out of range – should not happen for expected n */
    return products[(size_t)(n - 1)];
}

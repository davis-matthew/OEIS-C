/*
 * A073024  Primes p such that p-1 has a prime factor q > p^(2/3).
 * https://oeis.org/A073024
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that p-1 has a prime factor q > p^(2/3)
 * Range:     u8 none, u16 n <= 10, u32 n <= 993, u64 n <= 9586,
 *            u128 n <= 9586, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/floor_cube_root.h"

static idx_t sieve_limit = 0;
static uint32_t *spf = NULL;
static uint32_t *prime_list = NULL;
static size_t prime_cap = 0;
static size_t prime_cnt = 0;
static idx_t last_processed_prime_idx = 0;  /* 0‑based index of next prime to handle */

static seq_t *found = NULL;
static size_t found_cap = 0;
static size_t found_cnt = 0;

/* Build a linear sieve up to at least min_limit.  The old sieve is discarded
   and a fresh one is constructed.  The index of the first unprocessed prime
   is restored to the count of primes that were available before the rebuild. */
static void ensure_sieve(idx_t min_limit)
{
    if (sieve_limit >= min_limit)
        return;

    size_t old_cnt = prime_cnt;

    /* free previous data */
    if (spf) free(spf);
    if (prime_list) free(prime_list);

    sieve_limit = min_limit;
    spf = (uint32_t *)calloc((size_t)(sieve_limit + 1), sizeof(uint32_t));

    prime_cap = (size_t)(sieve_limit / 10 + 10);
    prime_list = (uint32_t *)malloc(prime_cap * sizeof(uint32_t));
    prime_cnt = 0;

    for (idx_t i = 2; i <= sieve_limit; ++i) {
        if (spf[i] == 0) {
            spf[i] = (uint32_t)i;
            if (prime_cnt == prime_cap) {
                prime_cap *= 2;
                prime_list = (uint32_t *)realloc(prime_list,
                                prime_cap * sizeof(uint32_t));
            }
            prime_list[prime_cnt++] = (uint32_t)i;
        }
        size_t curr_prime_cnt = prime_cnt;
        for (size_t j = 0; j < curr_prime_cnt; ++j) {
            uint32_t p = prime_list[j];
            idx_t prod = i * (idx_t)p;
            if (prod > sieve_limit)
                break;
            spf[prod] = p;
            if (p == spf[i])
                break;
        }
    }
    /* All primes up to old_cnt were already processed. */
    last_processed_prime_idx = old_cnt;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* If we already have enough terms, return the cached value */
    if (found_cnt >= n)
        return found[n - 1];

    /* Loop until we have found at least n terms */
    while (found_cnt < n) {
        /* Ensure we have a sieve large enough to supply more primes */
        if (sieve_limit == 0)
            ensure_sieve(200000);          /* initial size */
        else if (last_processed_prime_idx >= prime_cnt)
            ensure_sieve(sieve_limit * 2);  /* need more primes */

        /* Scan primes starting from the first unprocessed one */
        for (; last_processed_prime_idx < prime_cnt && found_cnt < n;
             ++last_processed_prime_idx) {
            uint32_t p = prime_list[last_processed_prime_idx];

            /* Factor p-1 using the precomputed smallest prime factors */
            idx_t m = (idx_t)p - 1;
            idx_t temp = m;
            seq_t max_factor = 0;
            while (temp > 1) {
                uint32_t f = spf[temp];
                if ((seq_t)f > max_factor)
                    max_factor = (seq_t)f;
                while (temp % f == 0)
                    temp /= f;
            }

            /* Threshold p^(2/3) = floor(cuberoot(p^2)) */
            seq_t sq = (seq_t)p * (seq_t)p;
            seq_t thr = floor_cube_root(sq);

            if (max_factor > thr) {
                if (found_cnt == found_cap) {
                    found_cap = found_cap ? found_cap * 2 : 4;
                    found = (seq_t *)realloc(found,
                                             found_cap * sizeof(seq_t));
                }
                found[found_cnt++] = (seq_t)p;
            }
        }
    }

    return found[n - 1];
}

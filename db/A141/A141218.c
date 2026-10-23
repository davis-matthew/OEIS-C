/*
 * A141218  Write the n-th nonprime (A018252(n)) as a product of primes;
 *          decrease one copy of the largest prime by 1 and increase one copy
 *          of the smallest prime by 1, multiply the resulting numbers.
 * https://oeis.org/A141218
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   not stated
 * Range:     u8 n <= 137, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   factor_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A141218:
   For the n-th nonprime number m (A018252(n)),
   factor m into primes p_i^e_i.
   Replace one copy of the largest prime factor by (p_max-1)
   and one copy of the smallest prime factor by (p_min+1),
   then take the product of all resulting factors.
   This implements the definition given in the OEIS entry. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* Static state for memoisation */
    static seq_t *cache  = NULL;   /* cache of computed terms (1‑based) */
    static idx_t cache_len = 0;   /* number of terms stored */
    static uint64_t curr_candidate = 1; /* last tested integer */

    /* Initialise cache with first term if not yet done. */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;     /* allocation failed */
        cache[0] = (seq_t)1;            /* a(1) = 1 (nonprime = 1) */
        cache_len = 1;
    }

    /* Extend cache up to the requested index n. */
    while (cache_len < n) {
        /* Find the next nonprime integer > curr_candidate. */
        do {
            ++curr_candidate;
        } while (is_prime(curr_candidate));

        /* Factor the nonprime number. */
        uint64_t primes_arr[64];
        uint64_t exps_arr[64];
        size_t cnt = 0;
        factor_u64(curr_candidate, primes_arr, exps_arr, &cnt);

        /* Identify smallest and largest prime factors. */
        uint64_t smallest = primes_arr[0];
        uint64_t largest  = primes_arr[0];
        for (size_t i = 1; i < cnt; ++i) {
            if (primes_arr[i] < smallest) smallest = primes_arr[i];
            if (primes_arr[i] > largest)  largest  = primes_arr[i];
        }

        /* Compute the adjusted product. */
        seq_t prod = (seq_t)1;
        bool applied_minus = false; /* (largest-1) applied once */
        bool applied_plus  = false; /* (smallest+1) applied once */

        for (size_t i = 0; i < cnt; ++i) {
            uint64_t p = primes_arr[i];
            size_t   e = exps_arr[i];
            while (e > 0) {
                if (!applied_plus && p == smallest) {
                    prod *= (seq_t)(p + 1);
                    applied_plus = true;
                    --e;
                } else if (!applied_minus && p == largest) {
                    prod *= (seq_t)(p - 1);
                    applied_minus = true;
                    --e;
                } else {
                    prod *= (seq_t)p;
                    --e;
                }
            }
        }

        /* Store the computed term in the cache. */
        seq_t *new_cache = (seq_t *)realloc(cache, (cache_len + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;     /* allocation failed */
        cache = new_cache;
        cache[cache_len] = prod;
        ++cache_len;
    }

    return cache[n - 1];
}

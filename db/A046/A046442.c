/*
 * A046442  Semiprimes whose two prime factors are distinct in length.
 * https://oeis.org/A046442
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 57, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   digit_count
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th semiprime whose two prime factors have different
   numbers of decimal digits.  The sequence starts at a(1)=22.  The
   implementation builds a sieve of smallest prime factors up to a
   fixed limit and walks through the integers, collecting the
   qualifying semiprimes.  Results are cached, so later calls are
   instantaneous. */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_count.h"

seq_t a(idx_t n)
{
    static seq_t *cache    = NULL;   /* cached sequence values      */
    static size_t  cache_len = 0;    /* number of cached values     */
    static size_t  cache_cap = 0;    /* current allocation capacity */
    static uint32_t *spf = NULL;    /* smallest prime factor table */
    static size_t   spf_limit = 0;   /* size of spf table           */
    static uint64_t current   = 2;   /* next candidate number       */

    /* Initialisation of the SPF table (once).  The limit is chosen
       conservatively so that n up to ~10000 are covered. */
    if (spf == NULL) {
        spf_limit = 500000;                              /* 5e5 is ample for n≈10000   */
        spf = (uint32_t*)malloc(sizeof(uint32_t)*(spf_limit+1));
        for (size_t i = 0; i <= spf_limit; ++i) spf[i] = 0;

        for (size_t i = 2; i <= spf_limit; ++i) {
            if (spf[i] == 0) {          /* i is prime */
                spf[i] = (uint32_t)i;
                if ((uint64_t)i * (uint64_t)i <= spf_limit) {
                    for (uint64_t j = (uint64_t)i * (uint64_t)i;
                         j <= spf_limit; j += i) {
                        if (spf[(size_t)j] == 0) spf[(size_t)j] = (uint32_t)i;
                    }
                }
            }
        }
    }

    /* Extend the cache until it contains at least n elements. */
    if ((size_t)n > cache_len) {
        const size_t target = (size_t)n;

        while (cache_len < target) {
            /* Ensure the candidate number is within the SPF table. */
            if (current > spf_limit) {
                /* Grow the SPF table by doubling its size. */
                size_t new_limit = spf_limit * 2;
                uint32_t *new_spf = (uint32_t*)malloc(sizeof(uint32_t)*(new_limit+1));
                for (size_t i = 0; i <= new_limit; ++i) new_spf[i] = 0;

                for (size_t i = 2; i <= new_limit; ++i) {
                    if (new_spf[i] == 0) {
                        new_spf[i] = (uint32_t)i;
                        if ((uint64_t)i * (uint64_t)i <= new_limit) {
                            for (uint64_t j = (uint64_t)i * (uint64_t)i;
                                 j <= new_limit; j += i) {
                                if (new_spf[(size_t)j] == 0)
                                    new_spf[(size_t)j] = (uint32_t)i;
                            }
                        }
                    }
                }
                free(spf);
                spf = new_spf;
                spf_limit = new_limit;
            }

            uint64_t a = spf[current];           /* smallest prime factor   */
            uint64_t b = current / a;             /* other factor            */

            /* Need distinct prime factors and the other factor must be prime. */
            if (a == b || spf[b] != b) {
                ++current;
                continue;
            }

            /* Accept only if the two primes have different digit counts. */
            if (digit_count(a) != digit_count(b)) {
                /* Ensure enough capacity in the cache. */
                if (cache_len >= cache_cap) {
                    cache_cap = cache_cap ? cache_cap * 2 : 64;
                    cache = (seq_t*)realloc(cache, cache_cap * sizeof(seq_t));
                }
                cache[cache_len++] = (seq_t)current;
                ++current;
                continue;
            }

            ++current;   /* not a valid term, try next number */
        }
    }

    /* Offset 1: return the (n-1)-th element in zero‑based array. */
    return cache[(size_t)n - 1];
}

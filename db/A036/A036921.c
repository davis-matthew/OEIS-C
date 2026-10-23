/*
 * A036921  Numbers n such that digit sum of n equals digit sum of
 *          'juxtaposition' and 'sum' of its prime factors (counted with
 *          multiplicity).
 * https://oeis.org/A036921
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   digit_sum, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A036921:
   Numbers n such that digit_sum(n) = digit_sum(concatenated prime factors)
   = digit_sum(sum of prime factors), counting multiplicity.
   Implemented by brute‑force search, memoising results in a static cache. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/factor_u64.h"

seq_t a(idx_t n)
{
    /* static cache of discovered terms */
    static seq_t *cache   = NULL;
    static size_t cache_cap = 16;
    static size_t cache_size = 0;
    static idx_t  next_candidate = 2;      /* start searching from 2 */

    /* arrays for factorisation (enough for numbers up to 10⁶) */
    static uint64_t fac_pr[64];
    static uint64_t fac_ex[64];
    size_t fac_cnt;

    /* initialise cache */
    if (cache == NULL) {
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;     /* allocation failed */
    }

    /* keep generating candidate numbers until we have enough terms */
    while (cache_size < n) {
        /* factor the current candidate */
        factor_u64((uint64_t)next_candidate, fac_pr, fac_ex, &fac_cnt);

        /* skip prime numbers */
        if (!(fac_cnt == 1 && fac_ex[0] == 1)) {
            seq_t sum_primes = 0;
            seq_t sum_digits_of_concat = 0;
            for (size_t i = 0; i < fac_cnt; ++i) {
                seq_t p = (seq_t)fac_pr[i];
                seq_t e = (seq_t)fac_ex[i];
                sum_primes += p * e;
                sum_digits_of_concat += digit_sum(p) * e;
            }
            seq_t ds_candidate   = digit_sum((seq_t)next_candidate);
            seq_t ds_sum_primes  = digit_sum(sum_primes);

            if (ds_candidate == sum_digits_of_concat &&
                sum_digits_of_concat == ds_sum_primes) {
                /* store the discovered term */
                if (cache_size == cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                 cache_cap * sizeof(seq_t));
                    if (tmp == NULL) return 0;   /* allocation failed */
                    cache = tmp;
                }
                cache[cache_size++] = (seq_t)next_candidate;
            }
        }

        ++next_candidate;
    }

    return cache[n - 1];
}

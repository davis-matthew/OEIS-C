/*
 * A036926  Digit sum of 'even' number equals digit sum of 'sum' and
 *          'juxtaposition' of its prime factors (counted with multiplicity).
 * https://oeis.org/A036926
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n‑th even integer m such that digit_sum(m) =
 *            digit_sum( sum of prime factors of m ) = digit_sum(
 *            concatenation of prime factors of m )
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   digit_sum, factor_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
#include "../utils/factor_u64.h"

static seq_t *cache      = NULL;  /* cached sequence terms          */
static size_t cache_size = 0;     /* number of cached terms          */
static size_t cache_cap  = 0;     /* capacity of cache array         */
static idx_t  last_checked = 2;   /* last even number examined       */

/* Append a value to the cache, expanding the array if necessary */
static void cache_append(seq_t val)
{
    if (cache_size == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_size++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* unused (offset is 1) */

    while (cache_size < n) {
        last_checked += 2;                     /* next even integer */

        /* Digit sum of the candidate */
        seq_t ds_m = digit_sum((seq_t)last_checked);

        /* Factorization of the candidate */
        size_t cf_cnt = 0;
        uint64_t primes[32];
        uint64_t exps[32];
        factor_u64((uint64_t)last_checked, primes, exps, &cf_cnt);

        /* Sum of prime factors and digit sum of their concatenation */
        seq_t sum_factors = 0;
        seq_t ds_concat   = 0;

        for (size_t i = 0; i < cf_cnt; ++i) {
            seq_t p   = (seq_t)primes[i];
            seq_t exp = (seq_t)exps[i];

            sum_factors += p * exp;
            ds_concat   += digit_sum(p) * exp;
        }

        seq_t ds_sum = digit_sum(sum_factors);

        /* Check the required equality of digit sums */
        if (ds_m == ds_sum && ds_m == ds_concat) {
            cache_append((seq_t)last_checked);
        }
    }

    return cache[n - 1];
}

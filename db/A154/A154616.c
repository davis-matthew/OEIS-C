/*
 * A154616  Primes of the form (4*n^2-8*n-9)/3.
 * https://oeis.org/A154616
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes of the form (4*n^2-8*n-9)/3 The index n=1 corresponds to
 *            the first such prime (17). The function builds the sequence on
 *            demand and memoises the values
 * Range:     u8 n <= 4, u16 n <= 52, u32 n <= 43, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* The core function required by the build */
seq_t a(idx_t n)
{
    /* Defensive: ignore calls with n==0 (undefined for the sequence) */
    if (n == 0) return (seq_t)0;

    /* Static state to cache generated primes */
    static idx_t next_k = 4;                   /* last tested k (start before 5) */
    static seq_t *cache = NULL;                /* array of found primes */
    static size_t cache_len = 0;               /* number of primes stored */
    static size_t cache_cap = 0;               /* current allocation size */

    /* Grow the cache until it contains at least n primes */
    while (cache_len < (size_t)n)
    {
        /* advance k and skip values where the expression is not divisible by 3 */
        ++next_k;
        if (next_k % 3ULL == 1ULL) continue;

        /* compute (4*k^2 - 8*k - 9)/3 as seq_t */
        seq_t val = ((seq_t)4 * next_k * next_k
                     - (seq_t)8 * next_k
                     - (seq_t)9) / (seq_t)3;

        /* keep only primes */
        if (is_prime_big(val))
        {
            /* resize cache if needed */
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 8;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }

    /* Return the pruned prime at index n (1‑based) */
    return cache[n - 1];
}

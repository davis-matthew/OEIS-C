/*
 * A162003  Primes of the form 1 + (k+2)^2 + (k+4)^4, k>=0.
 * https://oeis.org/A162003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th prime number of the form 1 + (k+2)^2 + (k+4)^4,
 *            with k >= 0. Sequence is enumerated in increasing order of the
 *            primes obtained
 * Range:     u8 none, u16 n <= 3, u32 n <= 3, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

/* Utility functions provided by the test harness. */
#include "../utils/is_prime_big.h"
#include "../utils/grow_seq.h"

/* External linkage as required. */
seq_t a(idx_t n)
{
    /* Static cache of found primes. */
    static seq_t *prime_cache = NULL;   /* pointer to array of primes */
    static size_t cache_len  = 0;        /* number of primes stored */
    static size_t cache_cap  = 0;        /* allocated capacity of array */
    static idx_t  next_k     = 0;        /* next k-value to test */

    /* Special case: OEIS offset starts at 1; any n <= 0 returns 0. */
    if ((idx_t)n == 0)
        return 0;

    /* Expand the cache until it contains at least n primes. */
    while (cache_len < (size_t)n)
    {
        /* Compute candidate prime for current k. */
        seq_t kk = (seq_t)next_k;          /* k as seq_t */
        seq_t a  = kk + (seq_t)2;
        seq_t b  = kk + (seq_t)4;

        seq_t a_sq = a * a;                /* (k+2)^2       */
        seq_t b_sq = b * b;                /* (k+4)^2       */
        seq_t b4   = b_sq * b_sq;          /* (k+4)^4       */

        seq_t candidate = (seq_t)1 + a_sq + b4; /* 1 + (k+2)^2 + (k+4)^4 */

        /* Test primality. */
        if (is_prime_big(candidate))
        {
            /* Append to cache. */
            grow_seq(&prime_cache, &cache_cap, cache_len + 1);
            prime_cache[cache_len++] = candidate;
        }

        /* Advance k for the next iteration. */
        ++next_k;
    }

    /* Return the cached n-th prime (indexing from 1). */
    return prime_cache[(size_t)n - 1];
}

/*
 * A142935  Primes congruent to 23 mod 64.
 * https://oeis.org/A142935
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the nth prime congruent to 23 modulo 64
 * Range:     u8 n <= 2, u16 n <= 212, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;      /* cache of found primes */
static size_t cache_len = 0;     /* number of primes stored */
static size_t cache_cap = 0;     /* current capacity of the cache */
static idx_t next_k = 0;         /* next k for candidate 23 + 64*k */

/* Return the n-th prime that is ≡ 23 (mod 64).  n ≥ 1 */
seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n < 1 */
        return 0;

    /* Grow the cache until we have at least n elements */
    while (cache_len < (size_t)n)
    {
        idx_t candidate = 23 + 64 * next_k;
        ++next_k;

        if (is_prime(candidate))
        {
            seq_t val = (seq_t)candidate;

            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();    /* allocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }

            cache[cache_len++] = val;
        }
    }

    return cache[(size_t)n - 1];
}

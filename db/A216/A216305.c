/*
 * A216305  Values of k such that 10*k+1 and 10*k+9 alone are prime between
 *          10*k and 10*k+9.
 * https://oeis.org/A216305
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 1007, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A216305:
   k such that 10*k+1 and 10*k+9 are the only primes in the interval [10*k, 10*k+9] */

#include <stddef.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* Cache of computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms stored */
static size_t cache_cap = 0;   /* allocated capacity */

/* Resize cache to at least 'needed' elements. */
static void ensure_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap < needed)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined offset; safe value */
        return 0;

    /* Extend cache until it contains at least n terms */
    while (cache_len < n)
    {
        /* Start candidate k just after the last cached term */
        idx_t k = (cache_len == 0) ? 0 : (idx_t)(cache[cache_len - 1]) + 1;

        /* Find the next k satisfying the condition */
        for (;; ++k)
        {
            idx_t a1 = 10 * k + 1;
            idx_t a9 = 10 * k + 9;

            if (!is_prime(a1) || !is_prime(a9))
                continue;           /* one of the required primes missing */

            /* Ensure no other primes in the interval */
            bool ok = true;
            for (idx_t x = 10 * k + 2; x <= 10 * k + 8; ++x)
            {
                if (is_prime(x))
                {
                    ok = false;
                    break;
                }
            }

            if (ok)
            {
                /* Store found k */
                ensure_cache(cache_len + 1);
                cache[cache_len++] = (seq_t)k;
                break;
            }
        }
    }

    return cache[n - 1];
}

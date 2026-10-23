/*
 * A024905  Numbers k such that 7*k + 1 is prime.
 * https://oeis.org/A024905
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = k such that 7*k + 1 is prime. The function searches 7*k+1
 *            for primes sequentially and memoises the found k‑values
 * Range:     u8 n <= 8, u16 n <= 970, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static idx_t   *cache = NULL;   // cached k-values (unsigned 64‑bit)
static size_t  cache_len = 0;   // number of cached values
static size_t  cache_cap = 0;   // current allocation size

seq_t a(idx_t n)
{
    /* Undefined for n < 1 – return 0 for safety. */
    if (n == 0)
        return (seq_t)0;

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        idx_t k = (cache_len == 0) ? (idx_t)1
                                   : cache[cache_len - 1] + (idx_t)1;

        for (;; ++k)
        {
            seq_t val = (seq_t)7 * k + (seq_t)1;             /* 7*k + 1 */
            if (is_prime_big(val))                          /* primality test */
            {
                /* Grow the cache if necessary. */
                if (cache_len == cache_cap)
                {
                    size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                    idx_t *tmp = (idx_t *)realloc(cache,
                                                new_cap * sizeof(idx_t));
                    if (!tmp)
                    {
                        /* Allocation failed – abort the program. */
                        abort();
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k; /* store the found k */
                break;                  /* move on to next n */
            }
        }
    }

    /* Return the n‑th term as seq_t. */
    return (seq_t)cache[n - 1];
}

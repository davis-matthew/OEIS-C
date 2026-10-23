/*
 * A159047  Primes which are triangular numbers plus 3.
 * https://oeis.org/A159047
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 28, u32 n <= 33, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A159047
    Formula: a(n) is the nth prime of the form T(k)+3 where T(k)=k(k+1)/2.
    The function searches successive triangular numbers, tests each
    T(k)+3 for primality with is_prime_big, and caches the results. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_prime_big.h"

static size_t  cache_count = 0;          /* number of primes stored */
static size_t  cache_cap   = 0;          /* current capacity of cache */
static seq_t  *cache_primes = NULL;     /* cached primes */
static idx_t  next_k = 0;                /* next triangular index to test */

seq_t a(idx_t n)
{
    /* Gather primes until we have at least n of them */
    while (cache_count < (size_t)n)
    {
        idx_t k = next_k++;                             /* triangular index */
        seq_t tri = ((seq_t)k) * (((seq_t)k) + 1) / 2;   /* T(k) in seq_t   */
        seq_t cand = tri + 3;                           /* candidate prime */

        if (is_prime_big(cand))
        {
            /* Grow cache when needed */
            if (cache_count >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap << 1 : 4;
                seq_t *tmp = (seq_t *)realloc(cache_primes,
                                              new_cap * sizeof(seq_t));
                if (!tmp) abort();                      /* allocation failed */
                cache_primes = tmp;
                cache_cap   = new_cap;
            }
            cache_primes[cache_count++] = cand;          /* store new prime   */
        }
    }

    /* Return the (n-1)-th stored prime (0‑indexed array) */
    return cache_primes[(size_t)n - 1];
}

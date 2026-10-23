/*
 * A138666  Numbers n such that every sum of consecutive positive numbers
 *          ending in n is not prime.
 * https://oeis.org/A138666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nice, nonn
 * Formula:   not stated
 * Range:     u8 n <= 118, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the nth term of OEIS A138666: numbers n such that
   n and 2n − 1 are both composite (i.e., not prime).  The function builds
   the sequence on the fly and memoises results so later calls are fast. */
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/is_prime.h"   /* bool is_prime(idx_t) */

/* static state: cached terms, buffer size, and next candidate number to test */
static seq_t *cache      = NULL;
static size_t cache_size = 0;   /* number of terms cached so far */
static size_t cache_cap  = 0;   /* allocated buffer capacity (in terms) */
static idx_t  candidate  = 1;   /* next integer to test for the condition */

/* Return the nth term (1‑based index). */
seq_t a(idx_t n)
{
    /* Extend the cache until it contains at least n terms. */
    while (cache_size < n)
    {
        /* Test whether the current candidate satisfies the condition. */
        bool ok = !is_prime(candidate) &&
                  !is_prime((idx_t)(candidate * 2u - 1u));

        if (ok)
        {
            /* Grow the buffer if necessary. */
            if (cache_size == cache_cap)
            {
                size_t new_cap = (cache_cap == 0) ? 16 : (cache_cap << 1);
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) /* allocation failure – abort execution. */
                    exit(EXIT_FAILURE);
                cache     = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = (seq_t)candidate;
        }
        candidate++;
    }
    return cache[n - 1];
}

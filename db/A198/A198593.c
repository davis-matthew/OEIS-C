/*
 * A198593  Odd numbers producing 10 odd numbers in the Collatz iteration.
 * https://oeis.org/A198593
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th odd integer x such that the Collatz trajectory *
 *            of x contains exactly 10 odd terms (including the starting
 *            number and 1)
 * Range:     u8 n <= 6, u16 n <= 396, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   count_odd_terms
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/count_odd_terms.h"

/* Returned value: the n-th term of the sequence.
 * 1‑based index: a(1) is the first term. */
seq_t a(idx_t n)
{
    if (n == 0)               /* undefined for 0, return 0 as a sentinel */
        return (seq_t)0;

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static idx_t last_checked = 1;  /* last odd number examined */

    /* initial allocation */
    if (cache == NULL)
    {
        cache_cap = 128;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    /* extend cache until it holds at least n terms */
    while (cache_len < (size_t)n)
    {
        /* next odd candidate */
        last_checked += 2;
        seq_t cand = (seq_t)last_checked;

        if (count_odd_terms(cand) == 10)
        {
            /* ensure enough capacity */
            if (cache_len == cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache,
                                              new_cap * sizeof(seq_t));
                if (!tmp)   /* graceful abort on allocation failure */
                {
                    free(cache);
                    cache = NULL;
                    cache_cap = cache_len = 0;
                    return (seq_t)0;
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = cand;
        }
    }

    return cache[n - 1];
}

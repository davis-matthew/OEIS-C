/*
 * A140544  Primes of form 17*k + 2.
 * https://oeis.org/A140544
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 408, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, free_seq_array, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Primes of form 17*k + 2, index starts at 1 (a(1) = 2). */
/* Implementation uses a growing cache; performs no large realloc because
   arbitrary‑precision seq_t objects may not be trivially relocatable. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static idx_t next_k     = 0;   /* next k to test */
    static seq_t *cache     = NULL;
    static idx_t  cache_len = 0;   /* number of primes stored */
    static idx_t  cache_cap = 0;   /* allocated capacity */

    /* grow the cache until it contains at least n primes */
    while (cache_len < n)
    {
        idx_t candidate = 17 * next_k + 2;  /* 17*k + 2 */
        if (is_prime(candidate))
        {
            if (cache_len >= cache_cap)
            {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = alloc_seq_array(new_cap);
                if (!tmp)
                {
                    /* allocation failed – terminate the program */
                    abort();
                }
                for (idx_t i = 0; i < cache_len; ++i)
                    tmp[i] = cache[i];
                free_seq_array(cache);
                cache = tmp;
                cache_cap = (idx_t)new_cap;
            }
            cache[cache_len++] = candidate;
        }
        ++next_k;   /* advance to next candidate */
    }

    return cache[n - 1];   /* a(1) is the first element */
}

/*
 * A044431  Numbers n such that string 9,9 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   contains99
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044431: numbers n such that the decimal representation of n
   contains the substring "99" but the representation of n-1 does not.
   Computed by an incremental search. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains99.h"

/* a(n): nth term of the sequence (offset 1) */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated slots in cache */
    static size_t cache_len = 0;   /* number of terms stored */

    /* If requested term is already cached, return it immediately */
    if (n <= cache_len)
        return cache[n - 1];

    /* start searching from the last known term + 1 */
    seq_t start = (cache_len == 0) ? 1 : (cache[cache_len - 1] + 1);

    /* grow cache if necessary */
    if (cache_len + 1 > cache_cap)
    {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* search until enough terms are found */
    for (seq_t x = start; cache_len < n; ++x)
    {
        if (contains99(x) && !contains99(x - 1))
        {
            /* store the found term */
            if (cache_len + 1 > cache_cap)
            {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }
    }

    return cache[n - 1];
}

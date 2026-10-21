/*
 * A087697  Numbers k such that k + 7 and k - 7 are both prime.
 * https://oeis.org/A087697
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th positive integer k such that k‑7 and k+7 are
 *            prime. The function generates terms on demand, caching results
 *            for efficiency
 * Range:     u8 n <= 17, u16 n <= 1069, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of found terms */
    static seq_t *cache = NULL;      /* array of k values */
    static size_t cache_len = 0;     /* number of elements stored */
    static size_t cache_cap = 0;     /* current allocated capacity */
    static idx_t last_k = 9;         /* last k examined; next candidate starts at 10 */

    /* Grow the cache until it contains at least n terms */
    while (cache_len < n)
    {
        /* Search forward for the next k satisfying the condition */
        do
        {
            ++last_k;
        } while (!is_prime(last_k - 7) || !is_prime(last_k + 7));

        /* Ensure there is room in the cache */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap ? cache_cap * 2 : 8;
            seq_t *new_arr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!new_arr) /* allocation failed – abort */
            {
                free(cache);
                cache = NULL;
                cache_len = cache_cap = 0;
                return (seq_t)0; /* undefined behaviour on failure */
            }
            cache = new_arr;
            cache_cap = new_cap;
        }

        /* Store the found k (truncated to seq_t) */
        cache[cache_len++] = (seq_t)last_k;
    }

    return cache[n - 1];
}

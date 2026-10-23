/*
 * A139529  Numbers k such that 24*k + 13 is prime.
 * https://oeis.org/A139529
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = k where 24*k + 13 is prime, with offset 1
 * Range:     u8 n <= 7, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/isPrimeGeneric.h"

static seq_t *cache = NULL;      /* cached k values (a(1), a(2), …) */
static idx_t cache_size = 0;     /* number of cached terms */
static idx_t cache_cap   = 0;     /* capacity of the cache array */

/* Double the cache capacity when needed. */
static void grow_cache(void)
{
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) abort();                    /* allocation failed */
    cache = tmp;
    cache_cap = (idx_t)new_cap;
}

/* Return the nth term of OEIS A139529 (k such that 24*k+13 is prime). */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* offset starts at 1 – guard */

    /* Ensure the cache contains at least n terms. */
    if (cache_size < n) {
        /* Start from the next integer after the last cached k. */
        idx_t k = cache_size ? (idx_t)cache[cache_size - 1] + 1 : 0;

        while (cache_size < n) {
            if (cache_cap <= cache_size) grow_cache();

            /* Candidate prime: 24*k + 13 */
            seq_t candidate = (seq_t)(24 * k + 13);

            if (isPrimeGeneric(candidate))
                cache[cache_size++] = (seq_t)k;

            ++k;
        }
    }

    return cache[n - 1];
}

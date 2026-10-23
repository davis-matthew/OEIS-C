/*
 * A142688  Primes congruent to 37 mod 57.
 * https://oeis.org/A142688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n-th prime congruent to 37 mod 57 (OEIS A142688)
 * Range:     u8 n <= 2, u16 n <= 191, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

/* External linkage required */
seq_t a(idx_t n)
{
    /* Static cache for the generated sequence */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static idx_t next_candidate = 37;          /* first number ≡37 (mod 57) */

    /* Expand cache to at least n elements */
    while (cache_len < n)
    {
        /* Search for the next prime of the form 57*k + 37 */
        while (!is_prime(next_candidate))
            next_candidate += 57;

        /* Grow the cache if necessary */
        if (cache_len >= cache_cap)
        {
            size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
            seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (tmp == NULL)
                return 0;           /* memory allocation failed; return 0 */
            if (cache != NULL)
                memcpy(tmp, cache, cache_len * sizeof(seq_t));
            free(cache);
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Store the prime and advance to the next candidate */
        cache[cache_len++] = next_candidate;
        next_candidate += 57;
    }

    return cache[n - 1];
}

/*
 * A087248  Squarefree abundant numbers.
 * https://oeis.org/A087248
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth squarefree abundant number * (squarefree AND
 *            sigma(n) > 2*n)
 * Range:     u8 n <= 7, u16 n <= 1546, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_squarefree, sigma_of
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/sigma_of.h"
#include "../utils/is_squarefree.h"

seq_t a(idx_t n)
{
    /* Static cache for the sequence. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity (in terms) */
    static idx_t next_candidate = 2;  /* next integer to test */

    /* Initial allocation if needed. */
    if (!cache) {
        cache_cap = 64;
        cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        if (!cache)
            return 0;   /* out of memory – not expected in tests */
    }

    /* Ensure capacity for new terms that might be appended. */
    while (cache_len < n) {
        /* Grow the cache array when it becomes full. */
        if (cache_len >= cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp)
                return 0;   /* out of memory – not expected in tests */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Test the current candidate. */
        if (is_squarefree((seq_t)next_candidate)) {
            seq_t sigma = sigma_of(next_candidate);
            seq_t twice = (seq_t)next_candidate * 2;
            if (sigma > twice) {
                cache[cache_len++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return cache[n - 1];
}

/*
 * A136014  Numbers n such that n*(n-1)-1 and n*(n+3)+1 are both prime.
 * https://oeis.org/A136014
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   numbers n such that n*(n-1)-1 and n*(n+3)+1 are both prime
 * Range:     u8 n <= 8, u16 n <= 33, u32 n <= 37, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime_big.h"

/* Nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    /* Static cache of found terms. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;   /* allocated capacity */
    static idx_t next_cur = 2;     /* next integer to test */

    /* Ensure the cache array is large enough. */
    if (cache_len >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* Allocation failed – abort with undefined result. */
            return 0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Generate terms until we have at least n of them. */
    while (cache_len < (size_t)n) {
        seq_t cur = (seq_t)next_cur;

        seq_t a_val = cur * ((seq_t)next_cur - 1) - 1; /* n*(n-1)-1 */
        seq_t b_val = cur * ((seq_t)next_cur + 3) + 1; /* n*(n+3)+1 */

        if (is_prime_big(a_val) && is_prime_big(b_val)) {
            cache[cache_len++] = cur;
        }

        ++next_cur;
    }

    return cache[(size_t)(n - 1)];
}

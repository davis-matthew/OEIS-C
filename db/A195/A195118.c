/*
 * A195118  Numbers with largest and smallest prime factors differing by 6.
 * https://oeis.org/A195118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 86, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Helpers:   _smallest_prime_factor, gpf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers whose largest and smallest prime factors differ by 6. */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Helper prototypes supplied by the build environment. */
#include "../utils/_smallest_prime_factor.h"
#include "../utils/gpf.h"

/* Return the nth term of OEIS A195118 (1‑based index). */
seq_t a(idx_t n)
{
    /* Static cache of already found terms. */
    static seq_t *cache      = NULL;   /* array of terms */
    static size_t cache_len  = 0;      /* number of terms stored */
    static size_t cache_cap  = 0;      /* capacity of the array */
    static uint64_t next_i   = 2;      /* next integer to test */

    /* Convert the requested index to a size_t for array indexing. */
    size_t target = (size_t)n;         /* n is 1‑based */

    /* Ensure the cache array can hold at least target terms. */
    if (cache_len + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* out‑of‑memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Keep searching until we have found target terms. */
    while (cache_len < target) {
        uint64_t spf = _smallest_prime_factor(next_i);
        uint64_t lpf = gpf(next_i);
        if (lpf - spf == 6) {
            /* Queue a new term; ensure space before writing. */
            if (cache_len + 1 > cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)next_i;
        }
        ++next_i;
    }

    return cache[n - 1];
}

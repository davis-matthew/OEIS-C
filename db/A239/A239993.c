/*
 * A239993  Numbers n such that n*sigma(n) is not divisible by 6.
 * https://oeis.org/A239993
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the nth positive integer m such that m*sigma(m) is not
 *            divisible by 6. a(1) = 1, a(2) = 4, a(3) = 7, …
 * Range:     u8 n <= 31, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   sigma_uint_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/sigma_uint_uint64.h"

/* Return the nth term of the sequence. */
seq_t a(idx_t n) {
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t next_candidate = 1;   /* next integer to test */

    /* ensure the cache is large enough for index n */
    if (cache == NULL) {
        cache_cap = 32;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }

    while (cache_len < n) {
        /* test next_candidate */
        uint64_t m = next_candidate;
        seq_t sigma = sigma_uint_uint64(m);

        bool has2 = (m % 2 == 0) || ((sigma % 2) == 0);
        bool has3 = (m % 3 == 0) || ((sigma % 3) == 0);

        if (!(has2 && has3)) {
            /* m*sigma(m) is NOT divisible by 6 -> include in sequence */
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)m;
        }
        ++next_candidate;
    }

    return cache[n - 1];
}

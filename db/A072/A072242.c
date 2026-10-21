/*
 * A072242  Smallest factorial containing exactly n 5's.
 * https://oeis.org/A072242
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the smallest factorial that contains exactly n decimal
 *            digits 5. The routine keeps a mutable cache so that calls with
 *            any order of n are fast
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 2, u128 n <= 4,
 *            big any n
 * Helpers:   digit_frequencies
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/digit_frequencies.h"

seq_t a(idx_t n)
{
    /* Sequence is defined with offset 1; return 0 for n = 0. */
    if (n == 0)
        return (seq_t)0;

    /* Static state shared by all calls. */
    static seq_t *cache  = NULL;   /* cache[count] = smallest factorial with that many 5's */
    static size_t  cap    = 0;      /* current capacity of cache */
    static idx_t   cur_m  = 1;      /* cur_m such that cache has processed factorial(cur_m!) */
    static seq_t   fact   = 1;      /* current value of cur_m! */

    /* Ensure cache is large enough for requested index n. */
    if (cap <= (size_t)n) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;   /* allocation failure */
        for (size_t i = cap; i < new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cap   = new_cap;
    }

    /* If we already know the answer, return it. */
    if (cache[n] != (seq_t)0)
        return cache[n];

    /* Helper to grow the cache safely. */
    #define ENSURE_CACHE_SIZE(min_needed)          \
        do {                                      \
            if (cap < (min_needed)) {             \
                size_t new_cap = (min_needed);    \
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t)); \
                if (!tmp) return (seq_t)0;        \
                for (size_t i = cap; i < new_cap; ++i) \
                    tmp[i] = (seq_t)0;            \
                cache = tmp;                      \
                cap   = new_cap;                  \
            }                                     \
        } while (0)

    /* Compute factorials until we have the required number of 5's. */
    while (cache[n] == (seq_t)0) {
        ++cur_m;
        fact *= (seq_t)cur_m;            /* update cur_m! */

        /* Count how many digit 5's the current factorial has. */
        unsigned int freq[10];
        digit_frequencies(fact, freq);
        idx_t count5 = freq[5];

        if (count5 > 0) {
            ENSURE_CACHE_SIZE((size_t)count5 + 1);
            if (cache[count5] == (seq_t)0)
                cache[count5] = fact;
        }
    }

    #undef ENSURE_CACHE_SIZE

    return cache[n];
}

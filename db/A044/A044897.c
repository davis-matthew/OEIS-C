/*
 * A044897  Base 11 run lengths alternate: even, odd, even,...
 * https://oeis.org/A044897
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   runs_alternate_even_first
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence implementation: A044897
   Base 11 run lengths alternate: even, odd, even,...
   A(n) is the n-th positive integer whose base‑11 representation
   consists of runs of identical digits whose lengths alternate in
   parity, starting with an even length run.
   Example: 12 (base 11 = "11") has a run of length 2 (even),
   then 134 (base 11 = "112") has runs of lengths 2 (even) and 1 (odd). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/runs_alternate_even_first.h"

seq_t a(idx_t n)
{
    /* lazy memoization: keep computed values in a static array */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_cap = 0;
    static seq_t candidate = 1;   /* next integer to test */

    if (n == 0) return 0;  /* offset is 1; behaviour below offset is undefined */

    /* grow cache as needed */
    while (cache_size < n) {
        /* find next integer that satisfies the property */
        while (!runs_alternate_even_first(candidate))
            ++candidate;

        /* add to cache */
        if (cache_size == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!new_cache) exit(EXIT_FAILURE);  /* allocation failed */
            for (size_t i = 0; i < cache_size; ++i)
                new_cache[i] = cache[i];
            free(cache);
            cache = new_cache;
            cache_cap = new_cap;
        }
        cache[cache_size++] = candidate;
        ++candidate;   /* prepare next candidate for future calls */
    }

    return cache[n - 1];
}

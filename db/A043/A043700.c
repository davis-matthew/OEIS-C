/*
 * A043700  Numbers whose base-11 representation has an even number of runs.
 * https://oeis.org/A043700
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑11 representation
 *            has an even number of runs
 * Range:     u8 n <= 122, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 85 terms through n = 8673 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Count runs in the base‑11 expansion of x. */
static unsigned int count_runs_base11(seq_t x)
{
    unsigned int runs = 0;
    int prev = -1;
    while (x > (seq_t)0) {
        int d = (int)(x % (seq_t)11);
        if (d != prev) {
            ++runs;
            prev = d;
        }
        x /= (seq_t)11;
    }
    return runs;
}

/* Return true iff the number of runs in x is even. */
static inline bool even_runs(seq_t x)
{
    return (count_runs_base11(x) & 1) == 0;
}

/* nth element of A043700, 1‑based. */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of qualifying numbers */
    static size_t cache_size = 0;     /* number of elements stored */
    static size_t cache_cap  = 0;     /* allocated capacity of cache */
    static seq_t next_candidate = 0;  /* next integer to test */

    /* Grow the cache until it contains at least n elements. */
    while (cache_size < (size_t)n) {
        ++next_candidate;              /* move to next integer */

        if (even_runs(next_candidate)) {
            /* Add to cache, resizing if necessary. */
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap << 1;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) exit(EXIT_FAILURE);
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_candidate;
        }
    }

    return cache[n - 1];
}

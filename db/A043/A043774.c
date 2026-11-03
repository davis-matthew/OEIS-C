/*
 * A043774  Numbers n such that number of runs in the base 3 representation of
 *          n is congruent to 1 mod 3.
 * https://oeis.org/A043774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 55 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A043774: a(n) = the nth integer whose base‑3 representation
 * has a number of runs ≡ 1 (mod 3).  The function memoizes results so
 * repeated calls, in any order, are efficient. */
#include <stdbool.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Helper: test whether a number satisfies the run‑condition. */
static bool sat_runs_mod3(seq_t v) {
    int runs = 0;
    int prev = -1;
    while (v > 0) {
        int d = (int)(v % 3);
        if (d != prev) {
            ++runs;
            prev = d;
        }
        v /= 3;
    }
    return (runs % 3) == 1;
}

seq_t a(idx_t n) {
    /* Minimal offset handling */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Static state for memoization */
    static seq_t *cache = NULL;      /* array of found terms */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_size = 0;    /* number of terms stored */
    static seq_t next_candidate = (seq_t)1;  /* next integer to test */

    /* Lazy initialization of cache */
    if (cache == NULL) {
        cache_cap = 16;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) {
            /* Allocation failure – return 0 as a graceful fallback */
            return (seq_t)0;
        }
    }

    /* Generate terms until we have at least n terms stored */
    while (cache_size < (size_t)n) {
        seq_t val = next_candidate;
        ++next_candidate;

        if (sat_runs_mod3(val)) {
            /* Ensure enough space in cache */
            if (cache_size >= cache_cap) {
                cache_cap = cache_cap * 2;
                seq_t *tmp = mem_resize(seq_t, cache, cache_cap);
                if (tmp == NULL) {
                    /* Allocation failure – skip further generation */
                    return cache[cache_size - 1];
                }
                cache = tmp;
            }
            cache[cache_size++] = val;
        }
    }

    return cache[(size_t)n - 1];
}

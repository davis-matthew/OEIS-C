/*
 * A043864  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 6 mod 9.
 * https://oeis.org/A043864
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Computes all integers whose base‑4 representation contains * a
 *            number of runs congruent to 6 (mod 9). a(1) is the first such *
 *            integer. The routine uses a growing cache so that calls in any *
 *            order are answered in O(1) after the value has been discovered
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   runs_mod9_eq6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/runs_mod9_eq6.h"

/* External integer types supplied by the build system */
extern seq_t a(idx_t n);   /* Forward declaration for consistency */

/* Static cache for the computed sequence elements */
static seq_t   *cache = NULL;      /* Array of cached values           */
static size_t   cache_len = 0;     /* Number of cached values          */
static size_t   cache_cap = 0;     /* Current capacity of cache        */
static uint64_t next_test = 1;     /* Next integer to inspect          */

/* Ensure the cache can hold at least n elements (1‑based index) */
static void ensure_cache_size(size_t n)
{
    if (cache_len >= n) return;

    /* Initial allocation */
    if (!cache) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);  /* Allocation failure */
    }

    /* Grow cache as needed */
    while (cache_len < n) {
        uint64_t val = next_test;
        if (runs_mod9_eq6(val)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) exit(EXIT_FAILURE);  /* Reallocation failure */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)val;
        }
        ++next_test;
    }
}

/* Main sequence accessor */
seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* Undefined for indices < 1 */
    ensure_cache_size((size_t)n);
    return cache[n - 1];
}

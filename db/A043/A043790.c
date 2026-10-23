/*
 * A043790  Number of runs in the base 3 representation of n is congruent to 5
 *          mod 6.
 * https://oeis.org/A043790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = n-th integer m such that the number of runs in the base‑3
 *            representation of m is congruent to 5 mod 6
 * Range:     u8 n <= 34, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   runs_mod6_eq5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/runs_mod6_eq5.h"

static seq_t *cache = NULL;        /* cache of computed sequence elements */
static size_t cache_cap = 0;       /* capacity of cache array */
static size_t cache_len = 0;       /* number of elements stored */
static seq_t last_checked = 0;     /* largest number tested so far */

/* Core function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, return 0 */
    /* Ensure cache holds at least n elements */
    while (cache_len < (size_t)n) {
        /* Expand cache if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            if (new_cap < (size_t)n) new_cap = n;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();    /* memory allocation failure */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Test successive integers starting after last_checked */
        seq_t candidate = last_checked + 1;
        while (!runs_mod6_eq5(candidate)) {
            ++candidate;
        }
        /* Store the candidate */
        cache[cache_len++] = candidate;
        last_checked = candidate;
    }
    return cache[n - 1];
}

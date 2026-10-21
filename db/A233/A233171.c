/*
 * A233171  Number of n X 5 0..5 arrays with no element x(i,j) adjacent to
 *          itself or value 5-x(i,j) horizontally, diagonally or
 *          antidiagonally, top left element zero, and 1 appearing before 2 3
 *          and 4, and 2 appearing before 3 in row major order (unlabeled
 *          6-colorings with no clashing color pairs).
 * https://oeis.org/A233171
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 15,
 *            big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A233171: number of n×5 0..5 arrays with no self‑adjacent or 5‑complement adjacency,
 *          top‑left element zero, and row‑major order constraints.
 *
 * Empirical linear recurrence (order 7):
 *   a(n) = 480·a(n-1) – 60416·a(n-2) + 1966080·a(n-3) + 22020096·a(n-4)
 *          – 1845493760·a(n-5) + 22548578304·a(n-6) – 68719476736·a(n-7)
 * Initial values (for n = 1 … 7):
 *   36, 8576, 2469888, 736362496, 221463445504,
 *   66799223701504, 20170789919653888
 *
 *  The function stores the first seven values in a static array and uses
 *  a static cache that grows with calls.  The helper `ensure_cache`
 *  (provided by the build system) guarantees that enough storage
 *  is allocated, and the memory is zero‑initialized.  The recurrence
 *  is evaluated using only the permitted arithmetic operators.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../utils/ensure_cache.h"

seq_t a(idx_t n)
{
    /* Guard: the sequence is defined for n ≥ 1 only. */
    if (n == 0) {
        return (seq_t)0;
    }

    /* Static array of the first seven values (zero‑based). */
    static const seq_t init[7] = {
        (seq_t)36,
        (seq_t)8576,
        (seq_t)2469888,
        (seq_t)736362496,
        (seq_t)221463445504,
        (seq_t)66799223701504,
        (seq_t)20170789919653888
    };

    /* Return the known value for n ≤ 7. */
    if (n <= 7U) {
        return init[n - 1];
    }

    /* Static cache for computed terms (1‑based indexing). */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static idx_t  computed_max = 0;

    /* Ensure the buffer can hold indices up to n.  +1 for 1‑based access. */
    ensure_cache(n + 1, &cache, &cache_cap);

    /* Initialise the cache with the first seven values if needed. */
    for (idx_t i = 1; i <= 7U && i <= n; ++i) {
        if (computed_max < i) {
            cache[i] = init[i - 1];
            computed_max = i;
        }
    }

    /* Compute terms from (computed_max + 1) up to n using the recurrence. */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        seq_t term = (seq_t)480 * cache[i - 1]
                   - (seq_t)60416 * cache[i - 2]
                   + (seq_t)1966080 * cache[i - 3]
                   + (seq_t)22020096 * cache[i - 4]
                   - (seq_t)1845493760 * cache[i - 5]
                   + (seq_t)22548578304 * cache[i - 6]
                   - (seq_t)68719476736 * cache[i - 7];
        cache[i] = term;
    }

    computed_max = n;
    return cache[n];
}

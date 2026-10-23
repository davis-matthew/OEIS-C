/*
 * A023163  Numbers k such that Fibonacci(k) == -2 (mod k).
 * https://oeis.org/A023163
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 429, u32 n <= 573, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   alloc_seq_array, fib_mod_calc, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS sequence A023163: numbers k such that Fibonacci(k) == -2 (mod k) */

/* The function searches for successive admissible k by testing the
   Fibonacci value modulo k using fast‑doubling.  Results are cached
   so that calls with arbitrary n are served in O(1) after the first
   search.  This implementation uses the utility allocator functions
   provided by the framework to avoid problematic calls to realloc
   when seq_t may be an arbitrary‑precision class. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"
#include "../utils/fib_mod_calc.h"

/* Return true when Fibonacci(k) ≡ -2 (mod k). */
static bool qualifies(idx_t k)
{
    seq_t fn, fn1;
    fib_mod_calc(k, (seq_t)k, &fn, &fn1);
    return ((fn + (seq_t)2) % (seq_t)k) == 0;
}

/* Cache for computed terms. */
static seq_t *cache      = NULL;   /* array of seq_t values */
static size_t capacity   = 0;      /* allocated size of cache   */
static size_t seen       = 0;      /* how many terms have been found */
static idx_t next_candidate = 1;   /* next k to test for qualification */

/* Ensure the cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (capacity >= need) return;

    size_t new_cap = capacity ? capacity << 1 : 128;
    if (new_cap < need) new_cap = need;

    seq_t *new_cache = alloc_seq_array(new_cap);
    if (!new_cache) abort();           /* allocation failed */

    if (cache) {
        /* Copy old data to new array. */
        for (size_t i = 0; i < seen; ++i) {
            new_cache[i] = cache[i];
        }
        /* Free old storage. */
        free_seq_array(cache);
    }

    cache    = new_cache;
    capacity = new_cap;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* sequence offset is 1 */

    while (seen < n) {
        /* Find next admissible k. */
        while (!qualifies(next_candidate)) ++next_candidate;

        /* Store it. */
        ensure_capacity(seen + 1);
        cache[seen++] = (seq_t)next_candidate;
        ++next_candidate;
    }
    return cache[n - 1];
}

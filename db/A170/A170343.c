/*
 * A170343  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^42 = I.
 * https://oeis.org/A170343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   linear recurrence of order 42 as derived from the generating
 *            function. * * a(0) = 1 * for 1 ≤ n ≤ 41: a(n) = 44 * Σ_{k=1}^{n}
 *            a(n‑k) + 2 * a(42) = 44 * Σ_{k=1}^{41} a(42‑k) + 1 * for n ≥ 43:
 *            a(n) = 44 * Σ_{k=1}^{41} a(n‑k) – 990 * a(n‑42) * * The sequence
 *            is memoised in a static array that grows...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 14,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc, free, EXIT_FAILURE */
#include <stdbool.h>  /* bool type */

/* Helpers for allocated seq_t arrays.  These are part of the build utilities. */
#include "../utils/alloc_seq_array.h"   /* alloc_seq_array(size_t) -> seq_t* */
#include "../utils/free_seq_array.h"    /* free_seq_array(seq_t*) */

static seq_t *cache = NULL;       /* Holds a(0)…a(max_cached) */
static size_t  cap   = 0;         /* Current capacity of cache in slots */
static idx_t   max_cached = 0;    /* Highest index that has been computed */
static bool    initialized = false;

/* Ensure that cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)n < cap) return;

    size_t new_cap = (cap == 0) ? 1 : cap * 2;
    while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until large enough */

    seq_t *tmp = alloc_seq_array(new_cap);
    if (!tmp) { /* allocation failed – exit */
        exit(EXIT_FAILURE);
    }

    /* Copy existing values into the new array. */
    if (cache) {
        for (size_t i = 0; i < cap; ++i)
            tmp[i] = cache[i];
        free_seq_array(cache);
    }

    cache = tmp;
    cap   = new_cap;
}

seq_t a(idx_t n)
{
    /* Initialise on first use. */
    if (!initialized) {
        ensure_capacity(0);
        cache[0] = (seq_t)1;                /* a(0) = 1 */
        max_cached = 0;
        initialized = true;
    }

    if (n <= max_cached) return cache[n];

    ensure_capacity(n);

    for (idx_t i = max_cached + 1; i <= n; ++i) {
        seq_t sum = 0;
        /* Compute the partial sum of the previous 41 values. */
        idx_t kmax = (i <= 42) ? i : 41;
        for (idx_t k = 1; k <= kmax; ++k)
            sum += cache[i - k];

        if (i <= 42) {
            seq_t p = (i == 0 || i == 42) ? (seq_t)1 : (seq_t)2;
            cache[i] = sum * (seq_t)44 + p;
        } else {
            cache[i] = sum * (seq_t)44 - cache[i - 42] * (seq_t)990;
        }
    }

    max_cached = n;
    return cache[n];
}

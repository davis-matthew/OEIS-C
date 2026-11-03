/*
 * A170597  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^48 = I.
 * https://oeis.org/A170597
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Validated: partial, pass, 18 terms through n = 17 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=1, for n≥1
 *   a(n) = num_coef(n) + 10 * Σ_{k=1}^{min(47,n)} a(n-k)
 *          – (n≥48 ? 55 * a(n-48) : 0)
 * where num_coef(0)=1, num_coef(1…47)=2, num_coef(48)=1, else 0.
 *
 * This implements the generating function of OEIS A170597.
 */

#include <stddef.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
#include "../utils/mem.h"

/* allocate an array of seq_t of given length (may be 0) */
static seq_t *seq_alloc_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void seq_free_array(seq_t* p)          { mem_free(p); }

/* Coefficient of t^n in the numerator polynomial */
static inline seq_t num_coef(idx_t n)
{
    if (n == 0)          return (seq_t)1;
    if (n <= 47)         return (seq_t)2;
    if (n == 48)         return (seq_t)1;
    return (seq_t)0;
}

/* Static cache for previously computed terms */
static seq_t *cache      = NULL;
static size_t cache_size = 0;
static idx_t last_computed = (idx_t)-1;   /* (UINT64_MAX) means none yet */

/* Main function required by the tests */
seq_t a(idx_t n)
{
    /* Return cached value if already computed */
    if (last_computed != (idx_t)-1 && n <= last_computed)
        return cache[n];

    /* Determine new cache size */
    size_t old_size = cache_size;
    size_t new_size = old_size;
    if ((size_t)n + 1 > new_size)
        new_size = (size_t)n + 1;

    /* Allocate a larger cache */
    seq_t *new_cache = seq_alloc_array(new_size);
    if (!new_cache)          /* Allocation failed */
        return (seq_t)0;

    /* Copy old values into the new cache */
    if (old_size > 0)
        mem_copy(new_cache, cache, old_size * sizeof(seq_t));

    /* Free the old cache */
    if (cache)
        seq_free_array(cache);

    /* Replace the old cache */
    cache = new_cache;
    cache_size = new_size;

    /* Initialize sliding window sum of the last up to 47 terms */
    seq_t window47 = (seq_t)0;
    idx_t start = (old_size >= 47) ? old_size - 47 : 0;
    for (idx_t i = start; i < old_size; ++i)
        window47 += cache[i];

    /* Compute terms from old_size up to n */
    for (idx_t i = old_size; i <= n; ++i) {
        seq_t val = num_coef(i);                 /* numerator coefficient */

        /* Add the 10 * window47 contribution */
        val += window47 * ((seq_t)10);

        /* Subtract 55 * a(i-48) if applicable */
        if (i >= 48)
            val -= cache[i-48] * ((seq_t)55);

        cache[i] = val;

        /* Update the sliding window for the next iteration */
        seq_t new_window = window47 + val;
        if (i >= 47)
            new_window -= cache[i-47];
        window47 = new_window;
    }

    last_computed = n;
    return cache[n];
}

/*
 * A188482  Diagonal sums of the Riordan matrix
 *          (1/(1-4x),(1-sqrt(1-4x))/(2*sqrt(1-4x))) (A188481).
 * https://oeis.org/A188482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   a(0)=1, a(1)=4, a(2)=17, a(3)=71
   For n >= 4:
   a(n) = ((7*n-9)*a(n-1) + (14-8*n)*a(n-2) + (23-15*n)*a(n-3) + (6-4*n)*a(n-4)) / (n-1)
   This follows from the OEIS recurrence given in the entry.
   The implementation uses memoisation so that calls in any order are O(1) after a
   single O(n) pass up to the maximum requested n.  All arithmetic is done using
   seq_t only.  No floating point or non‑standard types are used. */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* array of computed terms */
static idx_t cache_cap = 0;   /* allocated capacity */
static idx_t cache_size = 0;  /* number of computed terms (max index+1) */

/* Ensure the cache contains terms up to index n (inclusive). */
static void ensure_terms_up_to(idx_t n)
{
    if (cache_size > n)
        return;                /* already computed */

    /* Grow capacity if necessary. */
    idx_t needed = n + 1;
    if (cache_cap < needed) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < needed)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first four terms if they are not yet set. */
    if (cache_size < 4) {
        const seq_t base_vals[4] = { (seq_t)1, (seq_t)4, (seq_t)17, (seq_t)71 };
        idx_t i;
        for (i = cache_size; i < 4 && i <= n; ++i)
            cache[i] = base_vals[i];
    }

    /* Compute missing terms using the recurrence. */
    idx_t start = (cache_size > 3) ? cache_size : 4;
    idx_t i;
    for (i = start; i <= n; ++i) {
        /* Positive contribution from a(i-1). */
        seq_t pos = (seq_t)(7 * (int64_t)i - 9) * cache[i - 1];

        /* Negative contributions (we subtract later). */
        seq_t neg1 = (seq_t)(8 * (int64_t)i - 14) * cache[i - 2];
        seq_t neg2 = (seq_t)(15 * (int64_t)i - 23) * cache[i - 3];
        seq_t neg3 = (seq_t)(4 * (int64_t)i - 6) * cache[i - 4];

        seq_t term = pos - neg1 - neg2 - neg3;
        term /= (seq_t)(i - 1);   /* division is exact, integer result */

        cache[i] = term;
    }

    /* Update the logical size of the cache. */
    cache_size = n + 1;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    ensure_terms_up_to(n);
    return cache[n];
}

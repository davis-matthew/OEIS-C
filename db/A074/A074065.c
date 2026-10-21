/*
 * A074065  Numerators a(n) of fractions slowly converging to sqrt(3): let
 *          a(1) = 0, b(n) = n - a(n); if (a(n) + 1) / b(n) < sqrt(3), then
 *          a(n+1) = a(n) + 1, else a(n+1)= a(n).
 * https://oeis.org/A074065
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn
 * Formula:   a(n) = floor(n*(3 - sqrt(3)) / 2) Recurrence: a(1) = 0 a(n) =
 *            a(n-1) + 1 if (a(n-1)+1) / ((n-1) - a(n-1)) < sqrt(3) a(n) =
 *            a(n-1) otherwise
 * Range:     u8 n <= 24, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;     /* memoized values, 1‑based index */
static idx_t  max_computed = 0; /* highest index for which cache is filled */
static size_t cache_cap = 0;    /* allocated capacity of cache */

/* Ensure that cache holds values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (cache_cap < (size_t)(n + 1)) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)(n + 1)) new_cap <<= 1;
        cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Initialise first term if not yet done */
    if (max_computed == 0) {
        cache[1] = 0;
        max_computed = 1;
    }

    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t prev = cache[i - 1];
        idx_t prev_index = i - 1;                       /* n-1 in the recurrence */
        seq_t b = (seq_t)prev_index - prev;            /* b(n-1) = (n-1) - a(n-1) */
        seq_t lhs = (prev + 1) * (prev + 1);
        seq_t rhs = (seq_t)3 * b * b;
        if (lhs < rhs)
            cache[i] = prev + 1;
        else
            cache[i] = prev;
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* offset is 1; behaviour below it is irrelevant */
    ensure_cache(n);
    return cache[n];
}

/*
 * A196792  a(n) = A047848(10, n).
 * https://oeis.org/A196792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1; a(n) = 13*a(n-1) - 11 for n > 0 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_cache = NULL;        /* array of computed terms */
static idx_t a_cache_count = 0;      /* number of terms stored in a_cache */
static size_t a_cache_cap = 0;       /* allocated capacity of a_cache */

/* Ensure that terms up to index n are stored in a_cache */
static void a_ensure_cache(idx_t n)
{
    if (n < a_cache_count)
        return;

    /* Grow capacity if needed */
    if (a_cache_cap <= (size_t)n) {
        size_t new_cap = a_cache_cap ? a_cache_cap * 2 : 4;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        a_cache = (seq_t *)realloc(a_cache, new_cap * sizeof(seq_t));
        a_cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = a_cache_count; i <= n; ++i) {
        if (i == 0)
            a_cache[0] = (seq_t)1;
        else
            a_cache[i] = a_cache[i - 1] * (seq_t)13 - (seq_t)11;
    }
    a_cache_count = n + 1;
}

/* Return a(n) */
seq_t a(idx_t n)
{
    a_ensure_cache(n);
    return a_cache[n];
}

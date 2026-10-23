/*
 * A176386  x-values in the solution to x^2-74*y^2=1.
 * https://oeis.org/A176386
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7398*a(n-1) - a(n-2), a(1)=1, a(2)=3699
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 6, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc */

static seq_t *cache = NULL;     /* array of computed terms */
static size_t cache_size = 0;   /* number of terms currently in cache */
static size_t cache_cap  = 0;   /* allocated capacity in terms */

/* Computes the nth term of A176386.  Index n starts at 1. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;           /* undefined below offset */

    /* Convert index to a size_t for array indexing. */
    size_t idx = (size_t)n;

    /* Ensure the array has enough space for 'idx' terms. */
    if (cache_cap < idx) {
        size_t new_cap = idx;
        if (new_cap < 2) new_cap = 2;          /* at least two elements */
        else if (new_cap < 2 * cache_cap) new_cap = 2 * cache_cap;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;            /* memory exhaustion */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first two terms if they are not already present. */
    if (cache_size < 1) {
        cache[0] = (seq_t)1;          /* a(1) */
        cache_size = 1;
    }
    if (cache_size < 2) {
        cache[1] = (seq_t)3699;        /* a(2) */
        cache_size = 2;
    }

    /* Compute any terms that are missing up to the requested index. */
    for (size_t i = cache_size; i < idx; ++i) {
        cache[i] = cache[i-1] * ((seq_t)7398) - cache[i-2];
    }
    if (cache_size < idx) cache_size = idx;

    /* Return the requested term (1‑based index). */
    return cache[idx-1];
}

/*
 * A176075  Conway variant a(n) = 1+a(n-1-(n mod a(n-1))), with a(1)=1.
 * https://oeis.org/A176075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + a(n-1 - (n mod a(n-1))), a(1) = 1
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoization of the sequence values. */
static seq_t *cache    = NULL;   /* array with 1‑based indexing */
static size_t  cap     = 0;      /* current allocated capacity (in slots) */
static idx_t   max_n   = 0;      /* largest n for which cache[n] is already computed */

/* Ensure that the cache can index up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if ((size_t)(n + 1) <= cap)
        return;
    size_t new_cap = cap == 0 ? 16 : cap;
    while (new_cap <= (size_t)n)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL && new_cap != 0)
        abort();                     /* out of memory */
    cache = tmp;
    cap   = new_cap;
}

/* Conway variant a(n) = 1 + a(n-1 - (n mod a(n-1))), with a(1)=1. */
seq_t a(idx_t n)
{
    if (n == 0)                      /* undefined, but avoid crashes */
        return (seq_t)0;

    ensure_capacity(n);

    if (max_n == 0) {                /* initialise first term */
        cache[1] = (seq_t)1;
        max_n = 1;
    }

    if (n > max_n) {                 /* compute any missing terms up to n */
        for (idx_t k = max_n + 1; k <= n; ++k) {
            seq_t prev = cache[k - 1];
            idx_t m = k % (idx_t)prev;          /* n mod a(n-1) */
            idx_t idx = k - 1 - m;              /* index for the inner a() */
            cache[k] = cache[idx] + (seq_t)1;   /* a(k) = 1 + a(idx) */
        }
        max_n = n;
    }

    return cache[n];
}

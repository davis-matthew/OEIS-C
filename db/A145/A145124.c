/*
 * A145124  Numbers x such that there exists n in N : (x+1)^3-x^3=19*n^2.
 * https://oeis.org/A145124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n+2) = 302*a(n+1) - a(n) + 150
   Initial values: a(1) = 2, a(2) = 757
   The function returns the nth term (n ≥ 1) and memoizes results
   for arbitrary‑precision, 64‑bit, 32‑bit, etc. integer types. */

#include <stdlib.h>

/* Static cache for previously computed terms. */
static seq_t *cache     = NULL;   /* array of terms a(1), a(2), ... */
static idx_t cache_len = 0;       /* number of terms currently cached   */
static size_t cache_cap = 0;      /* allocated capacity of 'cache'       */

/* Ensure the cache can hold at least 'min_cap' elements. */
static void extend_cache(size_t min_cap)
{
    if (min_cap <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap : 16;
    while (new_cap < min_cap) new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = tmp;
    cache_cap = new_cap;
}

/* Return the nth term of A145124 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* offset is 1, so return 0 safely */

    /* Initialise base terms if needed. */
    if (cache_len < 2) {
        extend_cache(2);
        cache[0] = (seq_t)2;              /* a(1) */
        cache[1] = (seq_t)757;            /* a(2) */
        cache_len = 2;
    }

    if (n <= cache_len) {
        return cache[n - 1];              /* array index: a(1) at cache[0] */
    }

    /* Compute missing terms up to n. */
    extend_cache((size_t)n);
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        /* i-1 => a(i), i-2 => a(i-1), i-3 => a(i-2) */
        seq_t t1 = cache[i - 2];          /* a(i-1) */
        seq_t t0 = cache[i - 3];          /* a(i-2) */
        seq_t next = 302 * t1 - t0 + 150;
        cache[i - 1] = next;              /* store a(i) */
    }
    cache_len = n;

    return cache[n - 1];
}

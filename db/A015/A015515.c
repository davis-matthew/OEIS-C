/*
 * A015515  a(1) = 1, a(n) = Sum_{k=1..n-1} ((12^k - 1)/11)*a(k).
 * https://oeis.org/A015515
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((12^(n-1) + 10)/11) * a(n-1), a(1) = 1, a(2) = 1
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 7, u128 n <= 9,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Return the n-th term of OEIS A015515 (1‑based).  The function is
 * memoised; it works for any build where seq_t and idx_t are defined,
 * and for any order of calls. */
#include <stdlib.h>
#include <stdint.h>

/* Static cache: 1‑based indexing of already computed terms */
static seq_t   *cache      = NULL;   /* array of a(i) */
static size_t   cache_cap  = 0;      /* allocated capacity (including index 0) */
static idx_t    max_cached = 0;      /* largest n for which a(n) is stored */
static seq_t    pow12      = 0;      /* 12^(max_cached-1); valid when cache != NULL */

/* Ensure that the cache contains a(i) for all i ≤ n.
   If n ≤ max_cached nothing is done. */
static void ensure_cache(idx_t n)
{
    if (n <= max_cached)
        return;

    /* Initial allocation and base values (for n ≥ 2) */
    if (cache == NULL) {
        size_t new_cap = (size_t)n + 1U;
        if (new_cap < 8U)
            new_cap = 8U;
        cache = (seq_t *)malloc(sizeof(seq_t) * new_cap);
        if (cache == NULL)
            abort();                      /* allocation failure – wrap up */

        cache[0] = 0;                     /* unused, not part of the sequence */
        cache[1] = (seq_t)1;
        /* a(2) is defined as 1; we initialise it regardless of n */
        cache[2] = (seq_t)1;
        max_cached = 2;
        pow12      = (seq_t)12;           /* 12^(2-1) = 12^1 */
        cache_cap  = new_cap;
    }
    /* Grow the allocated array if necessary */
    else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2U;
        if (new_cap < (size_t)n + 1U)
            new_cap = (size_t)n + 1U;
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL)
            abort();
        cache   = tmp;
        cache_cap = new_cap;
    }

    /* Extend the cache up to n */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        /* 12^(i-1) = pow12 * 12   (current pow12 = 12^(i-2)) */
        seq_t next_pow12 = pow12 * (seq_t)12;
        seq_t factor = (next_pow12 + (seq_t)10) / (seq_t)11;
        cache[i] = factor * cache[i - 1];
        pow12 = next_pow12;          /* update for next iteration */
    }
    max_cached = n;
}

/* Public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        abort(); /* sequence starts at n=1 – invalid query */

    if (n <= 2)
        return (seq_t)1;                /* a(1) = a(2) = 1 */

    ensure_cache(n);
    return cache[n];
}

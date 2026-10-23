/*
 * A171231  a(n) = (10*2^n + 3 - (-1)^n)/6.
 * https://oeis.org/A171231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A171231
 * Formula: a(n) = (10*2^n + 3 - (-1)^n)/6
 * Recurrence (for n ≥ 3): a(n) = 2*a(n-1) + a(n-2) – 2*a(n-3)
 * Initial values: a(0)=2, a(1)=4, a(2)=7
 *
 * The implementation uses a simple memoisation array that grows
 * dynamically.  It is completely independent of any external utility
 * library; only the standard <stdlib.h> header is required.
 */

#include <stdlib.h>

static seq_t *cache = NULL;       /* memoised sequence values */
static idx_t  cache_cap = 0;      /* allocated array size   */
static idx_t  computed   = 0;     /* number of values already in the cache */

static void
ensure_cache(idx_t n)
{
    /* initialise the cache with the first three terms if it does not
       exist yet */
    if (cache_cap == 0) {
        cache_cap = 3;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) { /* allocation failed – aborting */
            abort();
        }
        cache[0] = (seq_t)2;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)7;
        computed = 3;
    }

    /* grow the array if it is too small for n+1 elements */
    if (n + 1 <= cache_cap)
        return;
    idx_t new_cap = cache_cap * 2;
    while (new_cap <= n)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) { /* allocation failed – aborting */
        abort();
    }
    cache      = tmp;
    cache_cap  = new_cap;
}

/**
 * Compute the n-th term of OEIS sequence A171231.
 *
 * @param n the zero‑based index (non‑negative)
 * @return the value a(n) as a seq_t
 */
seq_t
a(idx_t n)
{
    /* make sure the cache is large enough */
    ensure_cache(n);

    /* compute any missing terms up to n using the recurrence */
    while (computed <= n) {
        /* a(i) = 2*a(i-1) + a(i-2) – 2*a(i-3) */
        cache[computed] =
            cache[computed - 1] * (seq_t)2
          + cache[computed - 2]
          - cache[computed - 3] * (seq_t)2;
        ++computed;
    }

    return cache[n];
}

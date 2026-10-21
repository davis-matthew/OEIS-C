/*
 * A231025  Number of n X 2 0..2 arrays x(i,j) with each element horizontally,
 *          vertically or antidiagonally next to at least one element with
 *          value (x(i,j)+1) mod 3 and at least one element with value
 *          (x(i,j)-1) mod 3, and upper left element zero.
 * https://oeis.org/A231025
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) + 8*a(n-2) - 4*a(n-4) + a(n-5), with initial
 *            values a(1)=0, a(2)=2, a(3)=12, a(4)=62, a(5)=344
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/*  Memoised recurrence for A231025                                       */
/* ------------------------------------------------------------------ */

static seq_t *cache = NULL;        /* array holding computed terms, 1‑based */
static size_t cache_cap = 0;       /* allocated capacity (>= max index + 1) */
static idx_t max_computed = 0;     /* largest n whose value is known */

/* Ensure the cache can hold values up to index n (inclusive). */
static void ensure_cache(idx_t n)
{
    if (n < (idx_t)cache_cap) return;          /* already large enough */

    /* Allocate a little bit more space to reduce reallocations. */
    size_t new_cap = (size_t)n + 2;
    seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (!tmp) return;                       /* allocation failed – let caller fail */

    if (cache) {
        memcpy(tmp, cache, sizeof(seq_t) * (max_computed + 1));
        free(cache);
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Initialise the first five terms of the sequence.  This routine
   runs only once. */
static void init_cache(void)
{
    if (max_computed >= 5) return;           /* already initialised */

    /* Ensure space for indices 0 … 5 (index 0 is unused). */
    ensure_cache(5);
    cache[1] = (seq_t)0;
    cache[2] = (seq_t)2;
    cache[3] = (seq_t)12;
    cache[4] = (seq_t)62;
    cache[5] = (seq_t)344;
    max_computed = 5;
}

/* ------------------------------------------------------------------ */
/*  External interface                                                   */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* behaviour below offset */

    init_cache();                             /* set first 5 terms */

    /* Grow the cache if needed. */
    ensure_cache(n);

    /* Return already‑computed term. */
    if (n <= (idx_t)max_computed) return cache[n];

    /* Compute missing terms using the recurrence.  Start at 6. */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t term = 4 * cache[i-1]
                   + 8 * cache[i-2]
                   - 4 * cache[i-4]
                   + cache[i-5];
        cache[i] = term;
    }
    max_computed = n;
    return cache[n];
}

/*
 * A234262  Number of (n+1) X (4+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234262
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) - 24*a(n-3) + 4*a(n-4) + 36*a(n-5) -
 *            24*a(n-6) with offset 1 and initial values: a(1)=244, a(2)=358,
 *            a(3)=560, a(4)=988, a(5)=1816, a(6)=3616
 * Range:     u8 n <= 1, u16 n <= 9, u32 n <= 22, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Base values 1..6; index 0 is unused. */
static const seq_t base[7] = {0, 244, 358, 560, 988, 1816, 3616};

static seq_t *cache = NULL;   /* 1‑based cache array */
static idx_t  cache_len = 0;  /* highest index computed so far */

/* Ensure that values up to n are present in cache. */
static void ensure_cache(idx_t n)
{
    if (n < 1)
        return;

    if (cache_len >= n)
        return;                  /* already present */

    /* Resize the cache to hold indices 0 … n. */
    idx_t new_len = n + 1;              /* include index 0 */
    seq_t *new_cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
    if (!new_cache) {
        exit(EXIT_FAILURE);           /* allocation failure */
    }

    /* Initialise base values up to 6 (or up to n if n < 6). */
    idx_t limit_base = (n < 6) ? n : 6;
    for (idx_t i = 1; i <= limit_base; ++i) {
        new_cache[i] = base[i];
    }

    /* Compute remaining terms using the recurrence. */
    idx_t start = (cache_len > 6) ? cache_len + 1 : (idx_t)7;
    for (idx_t i = start; i <= n; ++i) {
        seq_t pos = (seq_t)3 * new_cache[i-1]   /* 3*a(i-1) */
                  + (seq_t)6 * new_cache[i-2]   /* 6*a(i-2) */
                  + (seq_t)4 * new_cache[i-4]   /* 4*a(i-4) */
                  + (seq_t)36 * new_cache[i-5];/* 36*a(i-5) */
        seq_t neg = (seq_t)24 * new_cache[i-3]  /* 24*a(i-3) */
                  + (seq_t)24 * new_cache[i-6];/* 24*a(i-6) */
        new_cache[i] = pos - neg;              /* pos >= neg by definition */
    }

    cache     = new_cache;
    cache_len = n;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;               /* undefined for n < 1 */

    if (n <= 6)
        return base[n];

    ensure_cache(n);
    return cache[n];
}

/*
 * A237714  Expansion of (1 + x)/(1 - x^2 - 2*x^5).
 * https://oeis.org/A237714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=a(1)=a(2)=a(3)=a(4)=1, * a(n)=2*a(n-5)+a(n-2) for n ≥ 5 *
 *            (Expansion of (1 + x)/(1 - x^2 - 2*x^5))
 * Range:     u8 n <= 20, u16 n <= 39, u32 n <= 76, u64 n <= 150,
 *            u128 n <= 299, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>           /* for malloc / realloc / free */

static seq_t *cache      = NULL;   /* memoised values */
static size_t cache_len  = 0;      /* number of stored elements   */
static size_t cache_cap  = 0;      /* allocated capacity (in elements) */

/* Ensure the cache can hold indices up to 'n' (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cache_cap > (size_t)n) return;

    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap <= (size_t)n) new_cap <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                /* abort on allocation failure */
    cache = tmp;
    cache_cap = new_cap;
}

/* Public entry point for the sequence. */
seq_t a(idx_t n)
{
    /* First‑time initialisation of the first five values. */
    if (cache_len == 0)
    {
        ensure_capacity(4);           /* indices 0 … 4 */
        cache[0] = cache[1] = cache[2] = cache[3] = cache[4] = 1;
        cache_len = 5;                /* a[0] … a[4] stored */
    }

    /* Already computed value? */
    if (n < cache_len) return cache[n];

    /* Expand cache to the needed size. */
    ensure_capacity(n);

    /* Compute remaining terms up to n using the recurrence relation. */
    for (idx_t i = cache_len; i <= n; ++i)
        cache[i] = cache[i - 5] * 2 + cache[i - 2];

    cache_len = n + 1;                 /* updated number of stored entries */
    return cache[n];
}

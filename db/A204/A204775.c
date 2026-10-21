/*
 * A204775  Number of (n+2) X 3 0..2 arrays with no 3 X 3 subblock having
 *          three equal diagonal elements or three equal antidiagonal
 *          elements, and new values 0..2 introduced in row major order.
 * https://oeis.org/A204775
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 22*a(n-1) - 308*a(n-3) + 196*a(n-4) Initial values:
 *            a(1)=2592, a(2)=55296, a(3)=1180224, a(4)=25190406
 * Range:     u8 none, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached values (0‑based) */
static size_t cache_allocated = 0;   /* allocated cache size */
static idx_t max_computed = 0;       /* largest index computed, 1‑based */

/* Ensure cache can hold at least 'need' elements. */
static void ensure_capacity(size_t need)
{
    if (cache_allocated >= need)
        return;
    seq_t *new_cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
    if (!new_cache) { abort(); } /* allocation failed */
    cache = new_cache;
    cache_allocated = need;
}

/* Initialise the first four terms of the sequence. */
static void init_base(void)
{
    if (cache_allocated < 4)
        ensure_capacity(4);
    cache[0] = (seq_t)2592;          /* a(1) */
    cache[1] = (seq_t)55296;         /* a(2) */
    cache[2] = (seq_t)1180224;       /* a(3) */
    cache[3] = (seq_t)25190406;      /* a(4) */
    max_computed = 4;
}

/* Public function computing a(n). Calls may arrive in any order. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, harmless */

    if (max_computed == 0)
        init_base();

    if (n <= max_computed)
        return cache[n - 1];

    /* Need to compute terms from max_computed+1 up to n. */
    ensure_capacity((size_t)n);
    for (idx_t i = max_computed + 1; i <= n; ++i)
    {
        /* idx_t i is 1‑based; cache is 0‑based. */
        seq_t t1 = cache[i - 2] * (seq_t)22;      /* a(i-1) */
        seq_t t2 = cache[i - 4] * (seq_t)308;     /* a(i-3) */
        seq_t t3 = cache[i - 5] * (seq_t)196;     /* a(i-4) */
        cache[i - 1] = t1 - t2 + t3;
    }
    max_computed = n;
    return cache[n - 1];
}

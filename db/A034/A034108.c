/*
 * A034108  Fractional part of square root of a(n) starts with 2: first term
 *          of runs.
 * https://oeis.org/A034108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-5) - 2*a(n-6) + a(n-7) for n > 8.
 *            Initial values (1‑based): 5, 18, 28, 39, 52, 68, 85, 105
 * Range:     u8 n <= 13, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* memoized sequence values */
static size_t cache_cap = 0;     /* allocated capacity of cache (in terms of seq_t) */
static size_t max_computed = 0;  /* number of terms currently computed */

/* Ensure cache can hold at least n terms. */
static void ensure_cache_capacity(idx_t n)
{
    if ((size_t)n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < (size_t)n)
        new_cap = (size_t)n;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!new_cache)
        abort();                    /* allocation failed */
    cache = new_cache;
    cache_cap = new_cap;
}

/* Initialize the cache with the first eight terms. */
static void initialize_cache(void)
{
    if (cache != NULL)
        return;
    ensure_cache_capacity(8);
    cache[0] = (seq_t)5;   /* a(1) */
    cache[1] = (seq_t)18;  /* a(2) */
    cache[2] = (seq_t)28;  /* a(3) */
    cache[3] = (seq_t)39;  /* a(4) */
    cache[4] = (seq_t)52;  /* a(5) */
    cache[5] = (seq_t)68;  /* a(6) */
    cache[6] = (seq_t)85;  /* a(7) */
    cache[7] = (seq_t)105; /* a(8) */
    max_computed = 8;
}

/* Compute terms up to index n (1‑based). */
static void compute_up_to(idx_t n)
{
    if ((size_t)n <= max_computed)
        return;
    if (max_computed < 8)
        initialize_cache();
    ensure_cache_capacity(n);
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* i is the 1‑based index of the term to compute */
        seq_t next = 0;
        next += (seq_t)2 * cache[(size_t)i - 2];   /* 2*a(i-1)  */
        next -=  cache[(size_t)i - 3];              /* -a(i-2)   */
        next +=  cache[(size_t)i - 6];              /* +a(i-5)   */
        next -= (seq_t)2 * cache[(size_t)i - 7];   /* -2*a(i-6) */
        next +=  cache[(size_t)i - 8];              /* +a(i-7)   */
        cache[(size_t)i - 1] = next;
    }
    max_computed = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        abort(); /* sequence is defined for n >= 1 only */
    initialize_cache();
    compute_up_to(n);
    return cache[(size_t)n - 1];
}

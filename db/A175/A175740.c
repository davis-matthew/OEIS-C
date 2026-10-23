/*
 * A175740  Expansion of 1/(1 - x - x^10 - x^19 + x^20).
 * https://oeis.org/A175740
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-10) + a(n-19) - a(n-20), a(0) = a(1) = ... =
 *            a(9) = 1 (OEIS offset 0)
 * Range:     u8 n <= 35, u16 n <= 65, u32 n <= 126, u64 n <= 248,
 *            u128 n <= 491, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache     = NULL;   /* memoisation table */
static size_t cache_cap = 0;      /* allocated capacity */
static size_t cache_size = 0;     /* next free index */

/* Ensure the cache can hold indices 0 … n (inclusive). */
static void ensure_capacity(idx_t n)
{
    size_t need = (size_t)n + 1;
    if (need <= cache_cap)
        return;
    size_t newcap = cache_cap > 0 ? cache_cap : 16;
    while (newcap < need)
        newcap *= 2;
    seq_t *newptr = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
    if (!newptr)
        abort();                      /* fatal error */
    cache = newptr;
    cache_cap = newcap;
}

/* Initialise the first ten entries (a(0)…a(9) = 1) if they are not yet set. */
static void init_first_ten(void)
{
    if (cache_size >= 10)
        return;
    ensure_capacity(9);
    for (size_t i = 0; i < 10; ++i)
        cache[i] = (seq_t)1;
    cache_size = 10;
}

/* Main function returning a(n) with arbitrary‑order memoisation. */
seq_t a(idx_t n)
{
    if (n < 10)                       /* a(0)…a(9) = 1 */
        return (seq_t)1;

    ensure_capacity(n);
    init_first_ten();

    /* Compute values up to n if not already cached. */
    for (size_t i = cache_size; i <= (size_t)n; ++i)
    {
        seq_t v = cache[i - 1] + cache[i - 10];
        if (i >= 19)
            v += cache[i - 19];
        if (i >= 20)
            v -= cache[i - 20];
        cache[i] = v;
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}

/*
 * A046637  Number of cubic residues mod 9^n.
 * https://oeis.org/A046637
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1 a(n) = 9*a(n-1) - 6 if n % 3 ≠ 0 a(n) = 9*a(n-1) - 20
 *            if n % 3 = 0 for n ≥ 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

// Include only what's needed for memory allocation
#include <stdlib.h>

/* External function to compute the sequence value */
seq_t a(idx_t n)
{
    /* Cached values of a(k) for k < cache_len */
    static seq_t *cache = NULL;     /* array of computed terms */
    static size_t cache_len = 0;    /* number of terms stored (cache_len ≥ 1 if cache[0] exists) */
    static size_t cache_cap = 0;    /* allocated capacity of cache */

    /* If we already have a(n) cached, return it */
    if (n < cache_len)
        return cache[n];

    /* Need to extend the cache to at least n+1 entries */
    size_t needed = n + 1;
    if (cache_cap < needed)
    {
        /* Grow capacity: double until it fits the needed size */
        size_t new_cap = cache_cap > 0 ? cache_cap : 1;
        while (new_cap < needed)
            new_cap *= 2;

        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) /* in production code we would handle out‑of‑memory */
            return 0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first element if the table was empty */
    if (cache_len == 0)
        cache[0] = (seq_t)1;

    /* Compute missing values sequentially */
    for (idx_t i = cache_len; i <= n; ++i)
    {
        if (i == 0)
            continue; /* already set */

        seq_t prev = cache[i - 1];
        seq_t sub = ((i % 3) == 0) ? (seq_t)20 : (seq_t)6;
        cache[i] = prev * (seq_t)9 - sub;
    }

    cache_len = needed;
    return cache[n];
}

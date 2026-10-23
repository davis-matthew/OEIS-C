/*
 * A174765  y-values in the solution to x^2 - 19*y^2 = 1.
 * https://oeis.org/A174765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 340*a(n-1) - a(n-2) with a(1)=0, a(2)=39
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* Compute the n‑th term of OEIS sequence A174765. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;
    static idx_t  cache_cap = 0;   /* current allocated capacity (indices 0…cache_cap-1) */
    static idx_t  cache_len = 0;   /* highest index computed so far */

    /* We need storage for indices 0..n (index 0 is unused). */
    size_t need = (size_t)(n + 1);
    if (need < 3u) need = 3u;              /* at least enough for the base values */

    if (cache_cap < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        cache = tmp;
        cache_cap = need;
    }

    /* Initialise the first two terms on first use. */
    if (cache_len == 0) {
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)39;
        cache_len = 2;
    }

    /* If the requested index is already cached, return it. */
    if (n <= cache_len)
        return cache[n];

    /* Compute missing terms using the recurrence relation. */
    for (idx_t i = cache_len + 1; i <= n; ++i)
        cache[i] = ((seq_t)340 * cache[i - 1]) - cache[i - 2];

    cache_len = n;
    return cache[n];
}

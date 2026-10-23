/*
 * A041420  Numerators of continued fraction convergents to sqrt(226).
 * https://oeis.org/A041420
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(n) = 30*a(n-1)+a(n-2), a(0)=15, a(1)=451
 * Range:     u8 n <= 0, u16 n <= 2, u32 n <= 5, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute the n-th term of A041420.
 * seq_t and idx_t are defined by the build system.
 * The function memoizes past results to support arbitrary call order. */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached values */
    static size_t  cap    = 0;      /* allocated capacity */
    static idx_t   last   = (idx_t)-1; /* highest index currently computed */

    /* Handle the two base cases directly. */
    if (n == 0) return (seq_t)15;
    if (n == 1) return (seq_t)451;

    /* First use: allocate a minimal cache and seed the base values. */
    if (cache == NULL) {
        /* start with a small size that comfortably holds the base terms */
        cap    = 16;
        cache  = (seq_t*)malloc(cap * sizeof(seq_t));
        cache[0] = (seq_t)15;
        cache[1] = (seq_t)451;
        last   = 1;
    }

    /* If the requested index is already computed, just return it. */
    if (n <= last) return cache[n];

    /* Ensure the cache can hold the new index. */
    if ((size_t)(n + 1) > cap) {
        size_t new_cap = cap;
        while ((size_t)(n + 1) > new_cap) new_cap *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute the missing terms up to n using the recurrence. */
    for (idx_t i = last + 1; i <= n; ++i) {
        cache[i] = (seq_t)30 * cache[i-1] + cache[i-2];
    }
    last = n;

    return cache[n];
}

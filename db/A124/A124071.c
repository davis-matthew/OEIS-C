/*
 * A124071  a(1)=1. a(n) = GCD(n,a(n-1)) + GCD(n+1,a(n-1)).
 * https://oeis.org/A124071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=gcd(n,a(n-1))+gcd(n+1,a(n-1)) */

#include <stdlib.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle arbitrary ordering of calls with memoization. */
    static seq_t *cache = NULL;   /* 1‑based array, cache[i] = a(i) */
    static idx_t cap   = 0;       /* allocated size (excluding index 0) */
    static idx_t last  = 0;       /* highest index that has been computed */

    if (n == 0)                    /* index below offset – undefined */
        return 0;

    /* Ensure capacity of the cache */
    size_t needed = (size_t)n + 1; /* +1 because we use 1‑based indexing */
    if (cap < needed) {
        size_t new_cap = (cap == 0 ? needed : (size_t)(2 * cap));
        if (new_cap < needed)
            new_cap = needed;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) abort();          /* allocation failed */
        cache = new_cache;
        cap   = (idx_t)new_cap;
    }

    /* Compute values up to n if necessary */
    for (idx_t i = last + 1; i <= n; ++i) {
        if (i == 1) {
            cache[1] = (seq_t)1;
        } else {
            seq_t prev = cache[i - 1];
            seq_t g1   = gcd((seq_t)i, prev);
            seq_t g2   = gcd((seq_t)(i + 1), prev);
            cache[i]   = g1 + g2;
        }
    }
    last = n;
    return cache[n];
}

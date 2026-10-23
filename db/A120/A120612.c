/*
 * A120612  For n>1, a(n) = 2*a(n-1) + 15*a(n-2); a(0)=1, a(1)=1.
 * https://oeis.org/A120612
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (5^n + (-3)^n)/2, a(0) = 1, a(1) = 1, recurrence: a(n) =
 *            2*a(n-1) + 15*a(n-2) for n ≥ 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static idx_t  max_n = 0;  /* largest n for which cache[n] is computed */

    /* Initialise cache for n = 0 and n = 1 */
    if (!cache) {
        cap = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* out‑of‑memory guard */
        cache[0] = 1;
        cache[1] = 1;
        max_n   = 1;
    }

    /* If the requested n is already in cache, return it */
    if (n <= max_n)
        return cache[n];

    /* Grow the cache array if needed */
    if (n + 1 > cap) {
        size_t new_cap = cap << 1;           /* double the capacity */
        if (new_cap <= (size_t)n)
            new_cap = (size_t)n + 1;        /* at least n+1 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;          /* out‑of‑memory guard */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms using the recurrence */
    for (idx_t i = max_n + 1; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] + 15 * cache[i - 2];
    }

    max_n = n;
    return cache[n];
}

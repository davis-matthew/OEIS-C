/*
 * A033497  a(n) = 2*a(n-1) + a(floor(n/2)), with a(1) = 1, a(2) = 2, a(3) =
 *          4.
 * https://oeis.org/A033497
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + a(floor(n/2)); a(1)=1, a(2)=2, a(3)=4
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;          /* array indexed 1..cap (0 unused) */
    static idx_t cap   = 0;              /* allocated size - 1 */
    static idx_t maxn  = 0;              /* largest n computed so far */

    /* Ensure the cache is large enough for n */
    if (n > cap) {
        idx_t old_cap = cap;
        /* Grow to at least n (can double for future calls if desired) */
        idx_t new_cap = n;
        /* Reallocate, zero‑initialise any new elements */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failed: return 0 illusionally */
        /* If we had to grow, zero the new portion */
        if (new_cap > old_cap)
            memset(tmp + old_cap + 1, 0, (new_cap - old_cap) * sizeof(seq_t));
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise base terms if they are not already set */
    if (cap >= 1 && cache[1] == 0) cache[1] = 1;
    if (cap >= 2 && cache[2] == 0) cache[2] = 2;
    if (cap >= 3 && cache[3] == 0) cache[3] = 4;

    /* If we already computed up to n, just return the value */
    if (n <= maxn) {
        return cache[n];
    }

    /* Compute missing terms from (maxn+1) up to n */
    idx_t start = (maxn < 4) ? 4 : maxn + 1;
    for (idx_t i = start; i <= n; ++i) {
        /* Recurrence: a(i) = 2*a(i-1) + a(i/2) */
        /* All arithmetic performed in seq_t */
        cache[i] = cache[i - 1] * 2 + cache[i / 2];
    }
    maxn = n;

    return cache[n];
}

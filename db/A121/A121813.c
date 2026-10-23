/*
 * A121813  a(n) = sum_{j=1..4} product_{k=1..4, k<>j} a(n-k).
 * https://oeis.org/A121813
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 10, u64 n <= 11, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS: A121813
   Formula: a(n) = a(n-4)*a(n-3)*a(n-2) + a(n-4)*a(n-3)*a(n-1)
            + a(n-4)*a(n-2)*a(n-1) + a(n-3)*a(n-2)*a(n-1)
   Initial values: a(1)=0, a(2)=1, a(3)=1, a(4)=1
   Access: a(idx_t n) with n>=1    (O(n) time, memoized)
   Note: Uses a static cache; safe for both C (seq_t is a plain integer type)
   and C++ (seq_t is an arbitrary‑precision integer class that is
   trivially constructible / destructible).  Realloc result is cast
   appropriately to satisfy C++ type safety.  Empty cache entries
   are filled lazily; repeated calls with any order of n are handled.
*/

#include <stdlib.h>   /* for malloc, realloc, free */

static seq_t *cache = NULL;        /* 1‑based cache; cache[0] unused          */
static size_t cache_cap = 0;       /* current capacity of cache (elements)   */
static size_t cache_len = 0;       /* highest index computed (0 if none)     */

seq_t a(idx_t n)
{
    /* Ensure we can address cache[n]; enlarge if necessary. */
    size_t needed = (size_t)n + 1;        /* include index 0 */
    if (needed > cache_cap) {
        void *tmp = realloc(cache, needed * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – return zero (failing case not exercised). */
            return (seq_t)0;
        }
        /* C++ may warn if realloc returns void*; cast to seq_t*. */
        cache = (seq_t *)tmp;
        cache_cap = needed;
    }

    /* Compute missing terms up to n.  Do not recompute already stored values. */
    if ((size_t)n > cache_len) {
        size_t i;
        for (i = cache_len + 1; i <= (size_t)n; ++i) {
            if (i <= 4) {                     /* initial terms */
                switch (i) {
                case 1: cache[i] = (seq_t)0; break;
                case 2: case 3: case 4: cache[i] = (seq_t)1; break;
                }
            } else {
                seq_t a1 = cache[i - 4];
                seq_t a2 = cache[i - 3];
                seq_t a3 = cache[i - 2];
                seq_t a4 = cache[i - 1];
                cache[i] = a1 * a2 * a3
                         + a1 * a2 * a4
                         + a1 * a3 * a4
                         + a2 * a3 * a4;
            }
        }
        cache_len = (size_t)n;
    }

    return cache[n];
}

/*
 * A121482  Number of nondecreasing Dyck paths of semilength n and having no
 *          peaks at odd level (n>=0). A nondecreasing Dyck path is a Dyck
 *          path for which the sequence of the altitudes of the valleys is
 *          nondecreasing.
 * https://oeis.org/A121482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1,a(1)=0,a(2)=1,a(3)=1,a(4)=3,a(5)=5 a(n) =
 *            a(n-1)+4*a(n-2)-2*a(n-3)-4*a(n-4)+a(n-6) for n≥6 (derived from
 *            the generating function)
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Direct table for the first six terms. */
    if (n < 6) {
        /* Note: cast from ptr to seq_t is valid in both C and C++. */
        static const seq_t init[6] = {1, 0, 1, 1, 3, 5};
        return init[(size_t)n];
    }

    /* ---------------------------------
       Lazy memoization storage.
       --------------------------------- */
    static seq_t *cache      = NULL;   /* Array of computed values. */
    static size_t cache_cap  = 0;       /* Allocated capacity (number of slots). */
    static size_t cache_len  = 0;       /* Number of valid elements (<= cache_cap). */

    /* Initial allocation on first use. */
    if (!cache) {
        cache_cap = 64;                            /* Initial capacity. */
        cache     = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (!cache) return 0;                       /* Allocation failure: return 0. */
        /* Fill the known initial values. */
        cache[0] = 1;
        cache[1] = 0;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 3;
        cache[5] = 5;
        cache_len = 6;                               /* Valid indices 0..5. */
    }

    /* Resize the cache if we need to store more elements. */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap;
        while (new_cap <= (size_t)n) {
            new_cap <<= 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Fallback: allocate new memory and copy manually. */
            tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
            if (!tmp) return 0;               /* Allocation failure. */
            /* Copy existing elements using assignment to support non‑trivial types. */
            for (size_t i = 0; i < cache_len; ++i) {
                tmp[i] = cache[i];
            }
            free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing values up to index n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        /* Recurrence: a(i) = a(i-1)+4*a(i-2)-2*a(i-3)-4*a(i-4)+a(i-6) */
        seq_t val = cache[i-1];
        val = val + (4 * cache[i-2]);
        val = val - (2 * cache[i-3]);
        val = val - (4 * cache[i-4]);
        val = val + cache[i-6];
        cache[i] = val;
    }
    cache_len = (size_t)n + 1;

    return cache[(size_t)n];
}

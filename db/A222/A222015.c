/*
 * A222015  Number of n X 2 arrays of occupancy after each element stays put
 *          or moves to some horizontal or vertical neighbor, without move-in
 *          move-out straight through or left turns.
 * https://oeis.org/A222015
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 25*a(n-2) + 40*a(n-3) + a(n-4) - 42*a(n-5) -
 *            20*a(n-6) + 10*a(n-7) + 4*a(n-8) - a(n-9) with offset 1 and
 *            initial terms a(1)=3, a(2)=27, a(3)=188, a(4)=1363, a(5)=9838,
 *            a(6)=70994, a(7)=512456, a(8)=3698699, a(9)=26696370
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Compute a(n) for n >= 1.  (n==0 is undefined by the sequence offset.) */
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* cache[0..max_cached] holds a(0..max_cached) */
    static idx_t  max_cached = 0;    /* highest index that has been computed */
    static size_t cache_cap   = 0;    /* current allocated capacity (number of elements) */

    /* Handle index 0, which is outside the official offset. */
    if (n == 0)
        return (seq_t)0;

    /* Initial values (placeholders for 0..9).  The values are small enough
       to fit in uint64_t; we cast when assigning to cache. */
    static const uint64_t init_vals[10] = {
        0ULL,          /* a(0) is unused, but we keep it as 0 */
        3ULL,          /* a(1) */
        27ULL,         /* a(2) */
        188ULL,        /* a(3) */
        1363ULL,       /* a(4) */
        9838ULL,       /* a(5) */
        70994ULL,      /* a(6) */
        512456ULL,     /* a(7) */
        3698699ULL,    /* a(8) */
        26696370ULL    /* a(9) */
    };

    /* Allocate and initialise the cache on the first call. */
    if (cache == NULL) {
        cache_cap   = 10; /* we need indices 0..9 inclusive */
        cache       = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL)
            return (seq_t)0;   /* allocation failed; fall back to 0 */

        for (idx_t i = 0; i < 10; ++i)
            cache[i] = (seq_t)init_vals[i];

        max_cached = 9;
    }

    /* If the requested term is already computed, return it immediately. */
    if (n <= max_cached)
        return cache[n];

    /* Grow the cache if necessary.  Use realloc with a C‑style cast that is
       accepted by both C and C++.  If realloc fails, we simply return 0. */
    if ((idx_t)(cache_cap) <= n) {
        size_t new_cap = (size_t)(n + 1);   /* we need indices 0..n inclusive */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)
            return (seq_t)0;   /* memory allocation failed */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute successive terms up to n, using the recurrence. */
    for (idx_t i = max_cached + 1; i <= n; ++i) {
        cache[i] = (seq_t)3 * cache[i-1]
                 + (seq_t)25 * cache[i-2]
                 + (seq_t)40 * cache[i-3]
                 +          cache[i-4]
                 - (seq_t)42 * cache[i-5]
                 - (seq_t)20 * cache[i-6]
                 + (seq_t)10 * cache[i-7]
                 + (seq_t)4  * cache[i-8]
                 -          cache[i-9];
    }

    max_cached = n;
    return cache[n];
}

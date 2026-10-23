/*
 * A186456  Number of (n+1) X 4 binary arrays with every 2 X 2 subblock
 *          commuting with each of its horizontal and vertical 2 X 2 subblock
 *          neighbors.
 * https://oeis.org/A186456
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - 2*a(n-3) + 2*a(n-4) - a(n-5) for n > 9 Initial
 *            values: a(1) = 42, a(2) = 51, a(3) = 111, a(4) = 162, a(5) =
 *            270, a(6) = 362, a(7) = 590, a(8) = 846, a(9) = 1362 This
 *            routine memoises previously computed terms and extends the cache
 *            as needed. It is safe for arbitrary order of cal...
 * Range:     u8 n <= 4, u16 n <= 18, u32 n <= 45, u64 n <= 98, u128 n <= 200,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static const seq_t init[10] = {
        (seq_t)0,    // unused, a(0) not defined
        (seq_t)42,   // a(1)
        (seq_t)51,   // a(2)
        (seq_t)111,  // a(3)
        (seq_t)162,  // a(4)
        (seq_t)270,  // a(5)
        (seq_t)362,  // a(6)
        (seq_t)590,  // a(7)
        (seq_t)846,  // a(8)
        (seq_t)1362  // a(9)
    };

    /* Return early for small n */
    if (n == (idx_t)0) return (seq_t)0;   /* safe fallback */
    if (n <= (idx_t)9) return init[(size_t)n];

    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t max_cached = 0;  /* highest index computed so far */
    static size_t cap = 0;         /* current capacity of the cache */

    /* Initialise cache on first call */
    if (cache == NULL) {
        cap = 10;                       /* indices 0..9 are needed */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;  /* allocation failure */

        /* Fill known values */
        cache[0] = (seq_t)0;
        for (size_t i = 1; i <= 9; ++i) cache[i] = init[i];
        max_cached = 9;
    }

    /* Extend cache if necessary */
    if ((size_t)n > max_cached) {
        /* Ensure capacity at least n+1 */
        if ((size_t)n + 1 > cap) {
            cap = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* allocation failure */
            cache = tmp;
        }

        /* Compute values from max_cached+1 up to n */
        for (idx_t i = max_cached + 1; i <= n; ++i) {
            size_t idx = (size_t)i;           /* array index -> size_t */
            if (idx <= 9) {
                /* Already initialized, but keep for safety */
                cache[idx] = init[idx];
            } else {
                /* a(i) = 2*a(i-1) - 2*a(i-3) + 2*a(i-4) - a(i-5) */
                seq_t val = cache[idx - 1] + cache[idx - 1];                 /* 2*a(i-1) */
                val += cache[idx - 4] + cache[idx - 4];                     /* +2*a(i-4) */
                val -= cache[idx - 3] + cache[idx - 3];                     /* -2*a(i-3) */
                val -= cache[idx - 5];                                     /* -a(i-5) */
                cache[idx] = val;
            }
        }
        max_cached = (size_t)n;
    }

    return cache[(size_t)n];
}

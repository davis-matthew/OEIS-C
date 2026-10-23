/*
 * A183995  Number of (n+1) X 2 0..2 arrays with all 2 X 2 subblock sums the
 *          same.
 * https://oeis.org/A183995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 19, u64 n <= 39, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 6*a(n-1) - 60*a(n-3) + 85*a(n-4) + 150*a(n-5)
 *              - 360*a(n-6) + 396*a(n-8) - 216*a(n-9)
 * Initial values (a(1) … a(9)):
 *   81, 171, 361, 855, 2025, 5175, 13225, 35535, 95481
 * The result is computed on demand using the above recurrence.
 * The function is thread‑safe for repeated calls and works for any
 * size of seq_t (arbitrary‑precision integer class allowed by the
 * build system).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) is out of sequence; return 0 */
    if (n == 0)
        return (seq_t)0;

    /* initial nine terms, 1‑indexed */
    static const seq_t init[9] = {
        (seq_t)81,  (seq_t)171,  (seq_t)361,  (seq_t)855,
        (seq_t)2025, (seq_t)5175, (seq_t)13225, (seq_t)35535,
        (seq_t)95481
    };

    /* static cache of already computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t computed = 0;        /* highest index already stored (1‑based) */

    /* Ensure cache has room for at least n entries */
    if (n > cache_cap) {
        idx_t new_cap = (cache_cap > 0) ? (cache_cap << 1) : 4;
        if (new_cap < n)
            new_cap = n;

        /* realloc with explicit cast for C++ compatibility */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp)
            abort();                     /* out of memory */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t k = computed + 1; k <= n; ++k) {
        if (k <= 9) { /* base case */
            cache[k - 1] = init[k - 1];
        } else {
            /* Use 1‑based indexing: cache[i-1] == a(i) */
            seq_t val = 0;

            /* 6*a(k-1) */
            val += (seq_t)6 * cache[k - 2];

            /* -60*a(k-3) */
            val -= (seq_t)60 * cache[k - 4];

            /* +85*a(k-4) */
            val += (seq_t)85 * cache[k - 5];

            /* +150*a(k-5) */
            val += (seq_t)150 * cache[k - 6];

            /* -360*a(k-6) */
            val -= (seq_t)360 * cache[k - 7];

            /* +396*a(k-8) */
            val += (seq_t)396 * cache[k - 9];

            /* -216*a(k-9) */
            val -= (seq_t)216 * cache[k - 10];

            cache[k - 1] = val;
        }
    }

    computed = n;                      /* update highest computed index */
    return cache[n - 1];
}

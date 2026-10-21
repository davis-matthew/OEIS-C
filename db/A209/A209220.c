/*
 * A209220  Number of n X 4 0..1 arrays avoiding 0 0 0 and 0 1 0 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A209220
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) - a(n-4) + a(n-5) - a(n-6)
 *            (empirical recurrence, a(1)=9, a(2)=81, a(3)=100, a(4)=196,
 *            a(5)=324, a(6)=576, a(7)=1024, a(8)=1764)
 * Range:     u8 n <= 4, u16 n <= 14, u32 n <= 34, u64 n <= 73, u128 n <= 152,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#if defined(__cplusplus)
#   include <new>
#endif

static const seq_t base_vals[9] =
{
    0,      /* unused index 0 */
    9,      /* a(1) */
    81,     /* a(2) */
    100,    /* a(3) */
    196,    /* a(4) */
    324,    /* a(5) */
    576,    /* a(6) */
    1024,   /* a(7) */
    1764    /* a(8) */
};

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static size_t capacity = 0;
    static size_t last_computed = 0; /* last index for which cache[i] is valid */

    /* Ensure enough space in the cache to store up to index n */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* In a real system we might abort or handle the error,
               but for this sequence calculation we assume allocation succeeds. */
            return 0;
        }
        /* Construct any new elements if seq_t has a non‑trivial constructor */
#if defined(__cplusplus)
        for (size_t i = capacity; i < new_cap; ++i) {
            new(&tmp[i]) seq_t();
        }
#endif
        cache = tmp;
        capacity = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t i = last_computed + 1; i <= (size_t)n; ++i) {
        if (i <= 8) {
            cache[i] = base_vals[i];
        } else {
            seq_t val = cache[i - 1];
            val += cache[i - 2];
            val += cache[i - 3];
            val -= cache[i - 4];
            val += cache[i - 5];
            val -= cache[i - 6];
            cache[i] = val;
        }
    }
    last_computed = (size_t)n;
    return cache[(size_t)n];
}

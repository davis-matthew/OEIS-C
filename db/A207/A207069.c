/*
 * A207069  Number of 2 X n 0..1 arrays avoiding 0 0 1 and 0 1 0 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A207069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Number of 2×n binary arrays avoiding * 0 0 1 and 0 1 0
 *            horizontally, and * 0 0 1 and 1 0 1 vertically. * Recurrence
 *            (for n ≥ 11): * a(n) = 3*a(n-1) - 2*a(n-2) + 3*a(n-3) - 6*a(n-4)
 *            * + 3*a(n-7) + a(n-8) - a(n-10) * Initial values: * a(1)=4,
 *            a(2)=16, a(3)=36, a(4)=81, a(5)=196, * a(6)=441,...
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 55, u128 n <= 113,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms.  Index 0 is unused, used only to
     * make the 1‑based indexing convenient. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* current number of filled entries (max index+1) */
    static size_t cache_cap = 0;   /* allocated capacity */

    if (n == 0)            /* the sequence is defined for n ≥ 1 only */
        return (seq_t)0;

    /* Grow the cache if necessary. */
    if ((size_t)n + 1 > cache_cap) {
        size_t new_cap = (size_t)n + 1;
        void *tmp = realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp)
            exit(1);       /* allocation failed */
        cache = (seq_t *)tmp;
        cache_cap = new_cap;
    }

    /* Compute any missing terms up to n. */
    for (size_t i = cache_len; i <= (size_t)n; ++i) {
        switch (i) {
            case 0: cache[i] = (seq_t)0; break;
            case 1: cache[i] = (seq_t)4; break;
            case 2: cache[i] = (seq_t)16; break;
            case 3: cache[i] = (seq_t)36; break;
            case 4: cache[i] = (seq_t)81; break;
            case 5: cache[i] = (seq_t)196; break;
            case 6: cache[i] = (seq_t)441; break;
            case 7: cache[i] = (seq_t)961; break;
            case 8: cache[i] = (seq_t)2116; break;
            case 9: cache[i] = (seq_t)4624; break;
            case 10: cache[i] = (seq_t)10000; break;
            default:
                /* Positive terms of the recurrence */
                seq_t pos = 3 * cache[i - 1]          /* 3*a(n-1) */
                          + 3 * cache[i - 3]          /* 3*a(n-3) */
                          + 3 * cache[i - 7]          /* 3*a(n-7) */
                          +       cache[i - 8];       /*    a(n-8) */

                /* Negative terms of the recurrence */
                seq_t neg = 2 * cache[i - 2]          /* 2*a(n-2) */
                          + 6 * cache[i - 4]          /* 6*a(n-4) */
                          +       cache[i -10];       /*    a(n-10) */

                cache[i] = pos - neg;                /* guaranteed non‑negative */
        }
    }

    cache_len = (size_t)n + 1;
    return cache[n];
}

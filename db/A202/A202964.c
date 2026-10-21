/*
 * A202964  Number of arrays of 4 integers in -n..n with sum zero and adjacent
 *          elements differing in absolute value.
 * https://oeis.org/A202964
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) -3*a(n-2) +2*a(n-3) -3*a(n-4) +3*a(n-5) -a(n-6);
 *            initial values: a(1)=4, a(2)=28, a(3)=108, a(4)=268, a(5)=544,
 *            a(6)=972
 * Range:     u8 n <= 3, u16 n <= 23, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n) {
    /* Static cache for memoisation; indices are 0‑based for a(1) at cache[0] */
    static seq_t *cache = NULL;      /* array of a(1), a(2), ... */
    static size_t cache_cap = 0;     /* allocated capacity */
    static size_t cache_size = 0;    /* number of values currently stored */

    /* Initial terms for n = 1 … 6 */
    static const seq_t init_vals[6] = {4, 28, 108, 268, 544, 972};

    /* Not meaningful below the offset */
    if (n == 0) return 0;

    /* Grow capacity to at least n (allocation double‑sized for amortised linear cost) */
    while ((size_t)n > cache_cap) {
        size_t new_cap = (cache_cap == 0) ? 16 : cache_cap * 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);   /* memory exhaustion */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (size_t k = cache_size + 1; k <= (size_t)n; ++k) {
        if (k <= 6) {
            cache[k - 1] = init_vals[k - 1];
        } else {
            /* Use unsigned arithmetic safely:  sum positives, then subtract sum of negatives */
            seq_t pos = (seq_t)3 * cache[k - 2]     /* a(k-1) */
                    + (seq_t)2 * cache[k - 4]       /* a(k-3) */
                    + (seq_t)3 * cache[k - 6];      /* a(k-5) */
            seq_t neg = (seq_t)3 * cache[k - 3]     /* a(k-2) */
                    + (seq_t)3 * cache[k - 5]       /* a(k-4) */
                    + cache[k - 7];                 /* a(k-6) */
            cache[k - 1] = pos - neg;
        }
    }
    cache_size = (size_t)n;
    return cache[n - 1];
}

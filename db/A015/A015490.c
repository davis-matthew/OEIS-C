/*
 * A015490  a(0)=1, a(1)=5, a(n) = sum_{k=0}^{k=n-1} 5^k a(k).
 * https://oeis.org/A015490
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A015490: a(0)=1, a(1)=5, a(n)=∑_{k=0}^{n-1}5^k a(k) for n≥2.
   Cache results so that calls may come in any order. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache array for computed values */
    static seq_t *cache = NULL;
    static size_t  capacity = 0;   /* allocated number of elements */
    static idx_t  last = 0;       /* number of values computed (highest index +1) */

    /* Ensure space for indices up to n */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* Initialise cache[0] and cache[1] if needed */
    if (last == 0) {
        cache[0] = (seq_t)1;  /* a(0) */
        last = 1;
    }
    if (last == 1 && n >= 1) {
        cache[1] = (seq_t)5;  /* a(1) */
        last = 2;
    }
    if (last > n) return cache[n];

    /* Compute missing values up to n */
    for (idx_t i = last; i <= n; ++i) {
        seq_t sum = 0;      /* ∑_{k=0}^{i-1} 5^k a(k) */
        seq_t pow = 1;      /* 5^k */
        for (idx_t k = 0; k < i; ++k) {
            sum = sum + pow * cache[k];
            pow = pow * (seq_t)5;    /* next power of 5 */
        }
        cache[i] = sum;
    }

    last = n + 1;   /* we have computed up to index n */
    return cache[n];
}

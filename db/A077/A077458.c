/*
 * A077458  a(n) = (a(n-1)*a(n-2) + a(n-2)*a(n-3) + 1)/a(n-4).
 * https://oeis.org/A077458
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (a(n-1)*a(n-2)+a(n-2)*a(n-3)+1)/a(n-4) Initial values:
 *            a(1)=a(2)=a(3)=a(4)=1 (offset 1)
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 11, u64 n <= 14, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc, abort */

seq_t a(idx_t n)
{
    /* Handle indices below the offset arbitrarily */
    if (n < 1) return (seq_t)0;

    /* Static cache for memoisation */
    static seq_t *cache = NULL;      /* array of computed terms (1‑based) */
    static size_t capacity = 0;      /* number of elements allocated */
    static idx_t last_computed = 0;  /* greatest index with value stored */

    /* Ensure the array is large enough.  We always need space for
       indices 1…4, so the minimum capacity is 5 (index 0 unused). */
    size_t needed = n + 1;           /* n may be large, but <2^64 */
    if (needed < 5) needed = 5;
    if (needed > capacity) {
        seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        if (!tmp) abort();           /* fatal if allocation fails */
        cache = tmp;
        capacity = needed;
    }

    /* Initialise the first four terms once (executed on the first call). */
    if (last_computed < 4) {
        /* The array has at least 5 slots now. */
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)1;
        cache[4] = (seq_t)1;
        last_computed = 4;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t num = cache[i - 1] * cache[i - 2]
                  + cache[i - 2] * cache[i - 3]
                  + (seq_t)1;
        cache[i] = num / cache[i - 4];
    }
    last_computed = n;

    return cache[n];
}

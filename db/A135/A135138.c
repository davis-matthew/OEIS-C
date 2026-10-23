/*
 * A135138  a(n) = 5*a(n-2) + 2*a(n-3).
 * https://oeis.org/A135138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 28, u64 n <= 53, u128 n <= 103,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 5*a(n-2) + 2*a(n-3),  a(0)=0, a(1)=0, a(2)=1 */

#include <stdlib.h>

/* Compute the n-th term of OEIS A135138. */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms. */
    static seq_t *cache = NULL;      /* array holding computed terms      */
    static idx_t  computed = 0;      /* number of terms already filled    */
    static idx_t  cap      = 0;      /* allocated size of the cache       */

    /* Base cases handled directly. */
    if (n == 0)  return (seq_t)0;
    if (n == 1)  return (seq_t)0;
    if (n == 2)  return (seq_t)1;

    /* Ensure the cache exists and is large enough. */
    if (cache == NULL) {
        /* Allocate enough space for the requested index plus the first
         * three explicit terms. */
        cap = (n >= 3 ? n + 1 : 3);
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;          /* allocation failed */

        /* Initialise the first three terms. */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)1;
        computed = 3;
    } else if (cap <= n) {
        /* Grow the cache to accommodate the index n. */
        idx_t new_cap = n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cap = new_cap;
    }

    /* If the requested index is already computed, just return it. */
    if (n < computed)
        return cache[n];

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = computed; i <= n; ++i) {
        seq_t term1 = cache[i - 2] * (seq_t)5;
        seq_t term2 = cache[i - 3] * (seq_t)2;
        cache[i] = term1 + term2;
    }
    computed = n + 1;

    return cache[n];
}

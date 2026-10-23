/*
 * A123151  a(n) = A000124(n)*a(n-3) for n > 2, otherwise n!.
 * https://oeis.org/A123151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 29, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A123151
   Formula: a(n) = A000124(n) * a(n-3)   for n > 2
   Initial values: a(0)=1, a(1)=1, a(2)=2
   where A000124(n) = (n^2 + n + 2)/2   */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static memoisation cache */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;
    static idx_t max_computed = (idx_t)-1;   /* -1 = "not yet initialised" */

    /* --- Initialise cache on first call ----------------------------------- */
    if (cache == NULL) {
        cache_len = 3;
        /* Allocate raw storage.  The cast is required for C++ compilation. */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* out of memory */

        /* Fill first three values: factorials 0!,1!,2! */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;

        max_computed = 2;   /* we have computed up to index 2 */
    }

    /* --- If requested value already computed -------------------------------- */
    if (n <= max_computed) {
        return cache[n];
    }

    /* --- Make sure the cache is large enough -------------------------------- */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failed */
        cache = tmp;
        cache_len = new_len;
    }

    /* --- Compute missing entries up to n ------------------------------------ */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* i >= 3 due to guard above */
        seq_t ii = (seq_t)i;
        /* A000124(i) = (i^2 + i + 2)/2 */
        seq_t factor = (ii * ii + ii + (seq_t)2) / (seq_t)2;
        cache[i] = factor * cache[i - 3];
    }

    max_computed = n;
    return cache[n];
}

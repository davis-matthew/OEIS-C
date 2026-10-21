/*
 * A052726  E.g.f. (1-sqrt(1-4*x-4*x^2))/ (2*(1+x)).
 * https://oeis.org/A052726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 9, u64 n <= 14, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A052726
   D-finite recurrence (shifted):
   a(0) = 0, a(1) = 1, a(2) = 2
   a(n+3) = (4*n^3+12*n^2+8*n)*a(n)
            + (22*n+12+8*n^2)*a(n+1)
            + (3*n+3)*a(n+2)   for all n ≥ 0   */

/* This implementation memoises computed values in a static
   dynamically‑grown array so that repeated or out-of‑order calls
   are O(1) after the first computation of the necessary
   indices.  It works for both fixed‑size integer types and for
   arbitrary‑precision integer classes. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache storing a[0]..a[cache_len-1] */

    static seq_t *cache = NULL;   /* pointer to cached values */
    static idx_t  cache_len = 0;  /* number of values currently stored (== highest index + 1) */

    /* If we already have the requested term, just return it. */
    if (n < cache_len)
        return cache[n];

    /* Determine the new required size (indices 0..n). */
    idx_t new_len = n + 1;
    if (cache_len == 0) /* first time: allocate at least space for 3 initial terms */
        new_len = new_len > 3 ? new_len : 3;

    /* Reallocate memory and/or construct any new slots. */
    if (cache == NULL) {
        cache = (seq_t *)realloc(NULL, new_len * sizeof(seq_t));
    } else {
        /* Reallocation retains existing elements, but new slots are uninitialised.
           We simply reallocate and then initialise the new indices. */
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    }
    if (cache == NULL) abort(); /* allocation failed */

    /* Initialise the first three entries if they were not present. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
    } else if (cache_len == 1) {
        cache[1] = (seq_t)1;
    } else if (cache_len == 2) {
        cache[2] = (seq_t)2;
    }

    /* Compute any missing terms from cache_len up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* Base cases already handled. Compute only for i >= 3. */
        if (i < 3) continue;          /* a[0], a[1], a[2] are known */

        idx_t k = i - 3;              /* recurrence index */
        seq_t nn = (seq_t)k;

        seq_t f0 = (seq_t)4 * nn * nn * nn
                 + (seq_t)12 * nn * nn
                 + (seq_t)8  * nn;

        seq_t f1 = (seq_t)8  * nn * nn
                 + (seq_t)22 * nn
                 + (seq_t)12;

        seq_t f2 = (seq_t)3 * nn
                 + (seq_t)3;

        /* a(i) = f0*a(k) + f1*a(k+1) + f2*a(k+2) */
        cache[i] = f0 * cache[k]
                 + f1 * cache[k + 1]
                 + f2 * cache[k + 2];
    }

    cache_len = new_len;
    return cache[n];
}

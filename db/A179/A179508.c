/*
 * A179508  a(n) is the unique integer such that Sum_{k=0..p-1} b(k)/(-n)^k ==
 *          a(n) (mod p) for any prime p not dividing n, where b(0), b(1),
 *          b(2), ... are Bell numbers given by A000110.
 * https://oeis.org/A179508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1)*(1-n)+n+1; a(1) = 2 This implements the recurrence
 *            given in the OEIS entry, using a static cache so that calls with
 *            arbitrary order of n are efficient
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 13, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* malloc, realloc */

seq_t a(idx_t n)
{
    /* Handle the (unlikely) case when n == 0 by returning 0. */
    if (n == 0) return (seq_t)0;

    /* Static cache: cache[i] == a(i+1)  (0‑based indexing). */
    static seq_t *cache = NULL;
    static idx_t cached_size = 0;   /* number of terms stored */

    /* If we have already computed the requested term, return it. */
    if (n-1 < cached_size)
        return cache[n-1];

    /* Ensure the cache is large enough.  Grow by a factor of 2 to keep
       amortised O(1) grows while still allowing arbitrary order of calls. */
    idx_t new_size = n;
    if (new_size < cached_size * 2) new_size = cached_size * 2;
    if (new_size < 2) new_size = 2;

    cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!cache) return (seq_t)0;   /* Allocation failure – unlikely in tests. */

    /* Initialisation: a(1) = 2.  Only needed on the first run. */
    if (cached_size == 0) {
        cache[0] = (seq_t)2;
        cached_size = 1;
    }

    /* Compute successive terms using the recurrence until we reach n. */
    for (idx_t i = cached_size; i < n; ++i) {
        seq_t prev = cache[i-1];          /* a(i)          */
        seq_t idx  = (seq_t)(i+1);        /* current index */
        /* a(i+1) = a(i)*(1-idx) + idx + 1  */
        seq_t cur  = prev * ((seq_t)1 - idx) + idx + (seq_t)1;
        cache[i] = cur;
    }

    cached_size = n;
    return cache[n-1];
}

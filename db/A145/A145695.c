/*
 * A145695  Numbers X such that there exists Y in N with X^2=111*Y^2+37.
 * https://oeis.org/A145695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 7, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  a(1) = 74,  a(2) = 43586,
   a(n+2) = 590*a(n+1) - a(n)       (n ≥ 1) */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Return zero for an invalid index; the sequence is defined only for n ≥ 1 */
    if (n == 0) return (seq_t)0;

    /* Cache the computed terms.  Index i in the array corresponds to a(i+1). */
    static seq_t *cache = NULL;
    static size_t used = 0;   /* number of elements currently stored */
    static size_t cap  = 0;   /* capacity of the allocated array */

    /* Ensure we have the first two terms ready. */
    if (used == 0) {
        cap   = 2;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failed */
        cache[0] = (seq_t)74;            /* a(1) */
        cache[1] = (seq_t)43586;         /* a(2) */
        used  = 2;
    }

    /* Grow the cache until it contains the requested index. */
    while (used < (size_t)n) {
        if (used == cap) {
            size_t new_cap = cap * 2;
            if (new_cap < used + 1) new_cap = used + 1;   /* just in case */
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;   /* allocation failed */
            cache = tmp;
            cap   = new_cap;
        }
        /* Recurrence: a(k+2) = 590*a(k+1) - a(k) */
        seq_t prod = (seq_t)590 * cache[used - 1];
        cache[used] = prod - cache[used - 2];
        ++used;
    }

    /* Return the required term. */
    return cache[n - 1];
}

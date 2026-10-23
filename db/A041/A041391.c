/*
 * A041391  Denominators of continued fraction convergents to sqrt(210).
 * https://oeis.org/A041391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A041391: Denominators of continued fraction convergents to sqrt(210)
   Recurrence: a(n) = 58 * a(n-2) - a(n-4)  for n > 3
   Initial values: a(0)=1, a(1)=2, a(2)=57, a(3)=116
   (for all n >= 0) */
#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms */
    static seq_t *cache = NULL;
    static size_t  cap    = 0;   /* capacity of cache (number of elements) */
    static idx_t  computed = 0;  /* largest index already computed */

    /* Initialise cache on first call */
    if (!cache) {
        /* At least 4 entries are required for the recurrence */
        cache = (seq_t *)malloc(4 * sizeof(seq_t));
        if (!cache) abort();          /* cannot proceed without allocation */
        cap = 4;
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)57;
        cache[3] = (seq_t)116;
        computed = 3;
    }

    /* Grow the cache if necessary to hold index n */
    if ((idx_t)cap < n + 1) {
        size_t newcap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();             /* out of memory */
        cache = tmp;
        cap = newcap;
    }

    /* If the term is already computed, return it immediately */
    if (n <= computed)
        return cache[n];

    /* Compute missing terms in ascending order */
    for (idx_t i = computed + 1; i <= n; ++i) {
        cache[i] = (seq_t)58 * cache[i - 2] - cache[i - 4];
    }
    computed = n;

    return cache[n];
}

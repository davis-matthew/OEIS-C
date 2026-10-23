/*
 * A163227  Fibonacci-accumulation sequence.
 * https://oeis.org/A163227
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-2) - a(n-6)
 * Range:     u8 n <= 23, u16 n <= 45, u32 n <= 91, u64 n <= 183,
 *            u128 n <= 369, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Initial values: a(1)=a(2)=a(3)=1, a(4)=2, a(5)=2, a(6)=4          */

#include <stdlib.h>

static seq_t *seq_cache = NULL;   /* cache of computed terms, index 0 unused   */
static idx_t  seq_cache_top = 0;  /* highest n for which seq_cache[n] is valid */

seq_t a(idx_t n)
{
    /* Sequence is only defined for n >= 1.  For any other n we return 0. */
    if (n < 1) return (seq_t)0;

    /* Initialise cache with the first six terms if this is the first call. */
    if (!seq_cache) {
        seq_cache = (seq_t *)calloc(7, sizeof(seq_t));   /* indices 0..6 : 0 unused */
        if (!seq_cache) return (seq_t)0;  /* allocation failure */

        seq_cache[1] = (seq_t)1;
        seq_cache[2] = (seq_t)1;
        seq_cache[3] = (seq_t)1;
        seq_cache[4] = (seq_t)2;
        seq_cache[5] = (seq_t)2;
        seq_cache[6] = (seq_t)4;
        seq_cache_top = 6;
    }

    /* If we already have this term, just return it. */
    if (n <= seq_cache_top) return seq_cache[n];

    /* Grow the cache to contain up to index n, computing new values via recurrence. */
    seq_t *new_cache = (seq_t *)realloc(seq_cache, (n + 1) * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;  /* allocation failure */

    seq_cache = new_cache;
    for (idx_t i = seq_cache_top + 1; i <= n; ++i) {
        /* The recurrence uses terms that have been computed already. */
        seq_cache[i] = (seq_t)2 * seq_cache[i - 2] - seq_cache[i - 6];
    }
    seq_cache_top = n;

    return seq_cache[n];
}

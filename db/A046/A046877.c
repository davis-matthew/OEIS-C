/*
 * A046877  a(n) = a(n-2) + a(n-3), with a(0) = 3, a(1) = 2, a(2) = 6.
 * https://oeis.org/A046877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 36, u32 n <= 75, u64 n <= 154,
 *            u128 n <= 312, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A046877 – a(n) = a(n-2) + a(n-3)
 * with a(0)=3, a(1)=2, a(2)=6. */
#include <stdlib.h>   /* malloc, realloc, exit */

static seq_t *seq_cache = NULL;   /* array holding computed terms */
static size_t seq_cache_cap = 0;  /* current allocated capacity */
static idx_t seq_cache_last = (idx_t)-1;  /* index of last filled entry */

/* Compute a(n).  Memoises results so calls in any order are fast. */
seq_t a(idx_t n)
{
    /* initialise cache on the very first call */
    if (seq_cache == NULL)
    {
        /* need space for indices 0,1,2 */
        seq_cache_cap = 3;
        seq_cache = (seq_t *)malloc(seq_cache_cap * sizeof(seq_t));
        if (seq_cache == NULL) exit(EXIT_FAILURE);
        seq_cache[0] = 3;
        seq_cache[1] = 2;
        seq_cache[2] = 6;
        seq_cache_last = 2;
    }

    /* if we already have the value return it */
    if (n <= seq_cache_last)
        return seq_cache[n];

    /* ensure we have capacity for the new index */
    size_t new_cap = (size_t)n + 1;
    if (new_cap > seq_cache_cap)
    {
        seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) exit(EXIT_FAILURE);
        seq_cache = tmp;
        seq_cache_cap = new_cap;
        /* the new slots contain indeterminate data – we will overwrite them soon */
    }

    /* compute missing terms using the recurrence */
    for (idx_t i = seq_cache_last + 1; i <= n; ++i)
        seq_cache[i] = seq_cache[i - 2] + seq_cache[i - 3];
    seq_cache_last = n;
    return seq_cache[n];
}

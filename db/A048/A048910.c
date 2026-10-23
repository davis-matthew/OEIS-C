/*
 * A048910  Indices of 9-gonal numbers that are also square.
 * https://oeis.org/A048910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 30*a(n-2) - a(n-4) - 10 (n >= 5); Initial values: a(1)=1,
 *            a(2)=2, a(3)=18, a(4)=49
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 27, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* cache[n] holds a(n), cache[0] unused */
static idx_t cache_cap = 0;   /* highest index already allocated */

/* Return the n‑th term of sequence A048910 (indices of 9‑gonal numbers that are also square). */
seq_t a(idx_t n)
{
    if (n == 0)                     /* undefined, but return 0 for safety */
        return 0;

    /* initialise cache on first call with the four known values */
    if (cache == NULL) {
        /* allocate space for indices 0..4 (0 unused, 1..4 set) */
        cache = (seq_t *)calloc(5, sizeof(seq_t));
        if (cache == NULL) abort();          /* allocation failed */
        cache_cap = 4;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)18;
        cache[4] = (seq_t)49;
    }

    /* grow cache if needed */
    if (n > cache_cap) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (tmp == NULL) abort();           /* allocation failed */
        /* initialise new entries to zero */
        for (idx_t i = cache_cap + 1; i <= new_cap; ++i)
            tmp[i] = (seq_t)0;
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = 5; i <= n; ++i) {
        /* recurrence: a(i) = 30*a(i-2) - a(i-4) - 10 */
        cache[i] = (seq_t)30 * cache[i - 2] - cache[i - 4] - (seq_t)10;
    }

    return cache[n];
}

/*
 * A033141  Base-5 digits are, in order, the first n terms of the periodic
 *          sequence with initial period 1,0,0.
 * https://oeis.org/A033141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = floor(5^(n+2)/124)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence used: a(n) = 5*a(n-1) + a(n-3) - 5*a(n-4) */
/* Base terms (n = 1..4): 1, 5, 25, 126 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Initial terms for n = 1..4 */
    const seq_t initial[4] = { (seq_t)1,
                               (seq_t)5,
                               (seq_t)25,
                               (seq_t)126 };

    /* Static cache for memoised values */
    static seq_t *cache = NULL;
    static size_t cap   = 0;  /* allocated size of cache */
    static size_t len   = 0;  /* last computed index (1-based) */

    if (n <= 4U)   /* trivially return the base term */
        return initial[n - 1U];

    /* Ensure cache exists and has space for the first 4 terms */
    if (cache == NULL)
    {
        cap = (4U > n) ? 4U : n;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (cache == NULL) return 0;   /* allocation failure (unlikely) */
        cache[0] = initial[0];
        cache[1] = initial[1];
        cache[2] = initial[2];
        cache[3] = initial[3];
        len = 4U;
    }

    /* Grow cache if necessary */
    if (cap < (size_t)n)
    {
        size_t new_cap = cap << 1;
        if (new_cap < (size_t)n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;   /* allocation failure (unlikely) */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n using the recurrence */
    for (size_t i = len; i < (size_t)n; ++i)
    {
        /* i is a 0‑based index but corresponds to n = i+1 */
        cache[i] = (seq_t)5 * cache[i - 1]      /* 5 * a(n-1)   */
                 + cache[i - 3]                /* + a(n-3)     */
                 - (seq_t)5 * cache[i - 4];    /* - 5 * a(n-4) */
    }
    len = (size_t)n;

    return cache[n - 1U];
}

/*
 * A159450  Numerator of Hermite(n, 7/11).
 * https://oeis.org/A159450
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 1, a(1) = 14, a(n) = 14*a(n-1) - 242*(n-1)*a(n-2)  (n ≥ 2)  */
/* Sequence: Numerator of Hermite(n, 7/11) multiplied by 11^n */

#include <stdlib.h>
#include <stdint.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;      /* array of a(n) values          */
static size_t  cap   = 0;        /* allocated capacity in array    */
static idx_t   cur_max = 0;      /* largest n with cache[n] ready */

/* Initialise the cache up to at least n (allocates space for 0 and 1).  */
static void init_cache(idx_t n)
{
    size_t min_cap = 2;
    if ((size_t)n + 1 > min_cap)
        min_cap = (size_t)n + 1;

    cache = (seq_t*)malloc(min_cap * sizeof(seq_t));
    if (!cache) abort();          /* fatal if allocation fails     */

    cache[0] = (seq_t)1;          /* a(0) */
    cache[1] = (seq_t)14;         /* a(1) */
    cur_max = 1;
    cap = min_cap;
}

/* Expand the cache if it cannot hold index n.  */
static void ensure_cache(idx_t n)
{
    if (cache == NULL)
        init_cache(n);
    else if ((size_t)n + 1 > cap) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;   /* double until enough */

        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* fatal if reallocation fails  */
        cache = tmp;
        cap = new_cap;
    }
}

/* Return a(n) for any n ≥ 0.  */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* If the desired term is already cached, return it. */
    if (n <= cur_max)
        return cache[n];

    /* Compute missing terms up to n.  */
    for (idx_t i = cur_max + 1; i <= n; ++i) {
        /* a(i) = 14*a(i-1) - 242*(i-1)*a(i-2) */
        cache[i] = (seq_t)14 * cache[i - 1]
                 - (seq_t)242 * (seq_t)(i - 1) * cache[i - 2];
    }
    cur_max = n;
    return cache[n];
}

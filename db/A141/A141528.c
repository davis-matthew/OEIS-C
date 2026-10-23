/*
 * A141528  Expansion of -x/(1 + x + 41*x^2).
 * https://oeis.org/A141528
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -a(n-1) - 41*a(n-2), a(1) = 0, a(2) = -1
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 13, i64 n <= 26, i128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;      /* array of computed terms, 1‑based indexing */
static idx_t  cached_max = 0;    /* largest index currently stored */
static idx_t  cache_capacity = 0;/* allocated capacity */

/* ------------------------------------------------------------------ */
/*  Computes the nth term of OEIS sequence A141528                     */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* short‑cuts for the first two terms */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)-1;

    /* initialise cache once we need to compute beyond the trivial terms */
    if (cache == NULL) {
        cache_capacity = 3;        /* at least indices 0..3 */
        cache = (seq_t *)malloc((cache_capacity + 1) * sizeof(seq_t));
        cache[0] = 0;              /* dummy, unused */
        cache[1] = 0;              /* a(1) */
        cache[2] = -1;             /* a(2) */
        cached_max = 2;
    }

    /* grow the cache if necessary and compute new terms */
    while (cached_max < n) {
        /* double capacity if we are about to overflow */
        if (cached_max + 1 > cache_capacity) {
            cache_capacity *= 2;
            cache = (seq_t *)realloc(cache, (cache_capacity + 1) * sizeof(seq_t));
        }

        /* recurrence: a(k) = -a(k-1) - 41*a(k-2) */
        ++cached_max;
        cache[cached_max] = -cache[cached_max - 1] - ((seq_t)41) * cache[cached_max - 2];
    }

    return cache[n];
}

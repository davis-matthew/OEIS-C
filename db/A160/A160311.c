/*
 * A160311  Numerator of Hermite(n, 13/31).
 * https://oeis.org/A160311
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence A160311, OEIS entry: Numerator of Hermite(n,13/31).
   The sequence satisfies      a(0)=1,   a(1)=26,
                               a(n+2)=26*a(n+1) - 1922*(n+1)*a(n).  */
seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache      = NULL;
    static size_t  capacity  = 0;   /* allocated cache size    */
    static size_t  computed  = 0;   /* number of terms stored  */

    /* initialise cache with the first two terms */
    if (computed == 0) {
        capacity = 2;
        cache    = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) return 0;                  /* allocation failure */
        cache[0] = 1;
        cache[1] = 26;
        computed = 2;
    }

    /* grow cache if necessary */
    if ((size_t)n + 1 > capacity) {
        size_t new_cap = capacity ? capacity << 1 : 1;
        if (new_cap < (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;                    /* allocation failure */
        cache    = tmp;
        capacity = new_cap;
    }

    /* compute missing terms up to n */
    while (computed <= n) {
        idx_t next = computed;                      /* index to compute   */
        seq_t term_prev    = cache[computed - 1];
        seq_t term_prevprev= cache[computed - 2];
        seq_t factor       = (seq_t)(next - 1);     /* (next-1) as seq_t  */
        cache[computed] = 26 * term_prev
                        - 1922 * factor * term_prevprev;
        computed++;
    }

    return cache[n];
}

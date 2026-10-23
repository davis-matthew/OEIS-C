/*
 * A163495  a(0)=0, a(1)=1, a(2)=2. For n >= 3, a(n) = a(n-1) - min(a(n-2),
 *          a(n-3)).
 * https://oeis.org/A163495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 61, i16 n <= 136, i32 n <= 224, i64 n <= 224,
 *            i128 n <= 224, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A163495: a(0)=0, a(1)=1, a(2)=2,
 * for n ≥ 3, a(n) = a(n‑1) – min(a(n‑2), a(n‑3)) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static state */
    static seq_t *cache   = NULL;   /* computed terms          */
    static idx_t  cap     = 0;      /* current allocation size */
    static idx_t  computed = 0;     /* number of terms filled  */

    /* Ensure the array is large enough for index n */
    if (n + 1 > cap) {
        size_t new_cap = (size_t)(n + 1);
        if (new_cap < 3) new_cap = 3;            /* need first three values */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) abort();                 /* allocation failed   */
        cache = new_cache;
        cap   = (idx_t)new_cap;
    }

    /* Initialise the first three values if not already done */
    if (computed < 3) {
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)2;
        computed = 3;
    }

    /* Compute missing terms up to n */
    if (n >= computed) {
        for (idx_t i = computed; i <= n; ++i) {
            seq_t a1 = cache[i - 1];
            seq_t a2 = cache[i - 2];
            seq_t a3 = cache[i - 3];
            seq_t mn = (a2 < a3) ? a2 : a3;
            cache[i] = a1 - mn;
        }
        computed = n + 1;
    }

    return cache[n];
}

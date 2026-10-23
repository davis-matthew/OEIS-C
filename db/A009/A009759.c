/*
 * A009759  Expansion of (3 - 21*x + 4*x^2)/((x-1)*(x^2 - 6*x + 1)).
 * https://oeis.org/A009759
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ((1+√2)^(2*n+1)+(1-√2)^(2*n+1)-14)/4 * Recurrence: a(0) =
 *            -3, a(1) = 0, * a(n) = 6*a(n-1) - a(n-2) + 14 for n ≥ 2 * The
 *            recurrence uses only integer arithmetic and can generate all
 *            terms * of the sequence in O(n) time with O(n) memory
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 25, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* cached values */
    static size_t cap   = 0;        /* allocated capacity */
    static size_t len   = 0;        /* number of valid entries (last used index + 1) */

    /* We need storage for indices 0 … n */
    size_t target = (size_t)n + 1;

    /* Ensure the cache has enough capacity */
    if (cap < target) {
        size_t new_cap = cap ? cap * 2 : 4;
        if (new_cap < target) new_cap = target;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                  /* allocation failed */
        if (cache) {
            for (size_t i = 0; i < len; ++i) tmp[i] = cache[i];
            free(cache);
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Initialise the first two terms if necessary */
    if (len < 2) {
        cache[0] = (seq_t)(-3);
        cache[1] = (seq_t)0;
        len = 2;
    }

    /* Populate the cache up to n using the recurrence */
    while (len < target) {
        cache[len] = cache[len-1] * 6 - cache[len-2] + 14;
        ++len;
    }

    return cache[n];
}

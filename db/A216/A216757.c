/*
 * A216757  a(n) = 6*a(n-1) - 9*a(n-2) + 3*a(n-3).
 * https://oeis.org/A216757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 6*a(n-1) - 9*a(n-2) + 3*a(n-3) ; a(1)=0, a(2)=-3,
 *            a(3)=-15
 * Range:     i8 n <= 4, i16 n <= 8, i32 n <= 16, i64 n <= 33, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑based cache for computed terms          */
static idx_t cache_cap = 0;          /* allocated size (>= cache_len+1)          */
static idx_t cache_len = 0;          /* highest index whose value is stored      */

seq_t a(idx_t n)
{
    /* Handle the first three terms explicitly. */
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)(-3);
    if (n == 3) return (seq_t)(-15);

    /* Grow the cache if necessary. */
    if (cache == NULL || cache_cap <= n) {
        idx_t new_cap = n + 1;              /* 1‑based indexing, so +1 */
        if (new_cap < 4) new_cap = 4;       /* at least space for 1..3 */

#if defined(__cplusplus)
        /* C++: allocate with new[], copy old values and delete old buffer. */
        seq_t *tmp = new seq_t[new_cap];
        if (cache) {
            for (idx_t i = 1; i <= cache_len; ++i) {
                tmp[i] = cache[i];
            }
            delete[] cache;
        }
        cache = tmp;
        cache_cap = new_cap;
#else
        /* C: use realloc, cast the void* result to seq_t*. */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;         /* allocation failed – safe‑return */
        cache = tmp;
        cache_cap = new_cap;
#endif
        /* Initialize the base values if they are not yet present. */
        if (cache_len < 3) {
            cache[1] = (seq_t)0;
            cache[2] = (seq_t)(-3);
            cache[3] = (seq_t)(-15);
            cache_len = 3;
        }
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        cache[i] = 6 * cache[i - 1] - 9 * cache[i - 2] + 3 * cache[i - 3];
    }
    cache_len = n;

    return cache[n];
}

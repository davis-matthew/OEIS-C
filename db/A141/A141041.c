/*
 * A141041  a(n) = ((3 + 2*sqrt(3))^n + (3 - 2*sqrt(3))^n)/2.
 * https://oeis.org/A141041
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) + 3*a(n-2), a(0) = 1, a(1) = 3
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *seq_cache = NULL;   /* array of computed values    */
static size_t seq_cache_size = 0; /* allocated capacity (elements) */
static idx_t seq_last = 0;        /* highest index computed so far */

/* enlarge the cache to hold at least 'needed' elements */
static void seq_ensure_capacity(size_t needed)
{
    if (needed <= seq_cache_size)
        return;

    size_t new_cap = seq_cache_size > 0 ? seq_cache_size : 4;
    while (new_cap < needed)
        new_cap <<= 1;

#ifdef __cplusplus
    seq_t *tmp = new seq_t[new_cap];
    if (seq_cache) {
        /* copy existing values */
        for (size_t i = 0; i < seq_cache_size; ++i)
            tmp[i] = seq_cache[i];
        delete[] seq_cache;
    }
    seq_cache = tmp;
#else
    seq_t *tmp = (seq_t *)realloc(seq_cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();           /* allocation failure */
    seq_cache = tmp;
#endif
    seq_cache_size = new_cap;
}

seq_t a(idx_t n)
{
    /* initialise cache on first use */
    if (!seq_cache) {
        seq_ensure_capacity(4);
        seq_cache[0] = (seq_t)1;
        seq_cache[1] = (seq_t)3;
        seq_last = 1;
    }

    /* ensure space for the requested index */
    seq_ensure_capacity((size_t)n + 1);

    /* compute any values that are still missing */
    for (idx_t i = seq_last + 1; i <= n; ++i) {
        seq_cache[(size_t)i] =
            (seq_t)6 * seq_cache[(size_t)i - 1]
          + (seq_t)3 * seq_cache[(size_t)i - 2];
    }
    if (n > seq_last)
        seq_last = n;

    return seq_cache[(size_t)n];
}

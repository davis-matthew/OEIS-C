/*
 * A132342  a(n) = (a(n-1)*a(n-4)) - (a(n-2)*a(n-3)), with
 *          a(1)=a(2)=a(3)=a(4)=1.
 * https://oeis.org/A132342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = a(n-1)*a(n-4) - a(n-2)*a(n-3) with initial values
 *            a(1)=a(2)=a(3)=a(4)=1
 * Range:     i8 n <= 105, i16 n <= 105, i32 n <= 105, i64 n <= 105,
 *            i128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* array indexed from 1; cache[0] unused */
static size_t cache_len = 0;         /* current allocated length (array size) */

/* Return the n-th term of sequence A132342.  The function memoizes
   previously computed terms so that subsequent calls are fast, even
   when n is supplied in arbitrary order. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* out of bounds, optional safeguard */

    /* Required array size is n+1 (index 0 unused). */
    size_t required_len = (size_t)n + 1;
    if (cache_len < required_len) {
        /* Grow the cache to the needed size. */
        seq_t *tmp = (seq_t *)realloc(cache, required_len * sizeof(seq_t));
        if (!tmp) abort();               /* allocation failure */
        cache = tmp;
        cache_len = required_len;

        /* Compute all missing entries up to n. */
        idx_t start = (idx_t)(cache_len > 1 ? cache_len - 1 : 1);
        /* cache_len already updated, so compute from old_length to n. */
        for (idx_t i = start; i <= n; ++i) {
            if (i <= 4) {
                cache[i] = 1;
            } else {
                cache[i] = cache[i-1] * cache[i-4] - cache[i-2] * cache[i-3];
            }
        }
    }
    return cache[n];
}

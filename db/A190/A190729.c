/*
 * A190729  E.g.f. exp(x+1/6*x^3+1/24*x^4).
 * https://oeis.org/A190729
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 10, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A190729: E.g.f. exp(x+1/6*x^3+1/24*x^4)
   This implementation uses the linear recurrence
   +6*a(n) - 6*a(n-1) - 3*(n-1)*(n-2)*a(n-3)
   -(n-1)*(n-2)*(n-3)*a(n-4) = 0,
   with initial values
   a(0) = 1, a(1) = 1, a(2) = 1, a(3) = 2. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;    /* number of cached entries */

    /* initialise cache with the first four values */
    if (cache_len < 4) {
        size_t needed = 4;
        if (cache == NULL) {
            cache = (seq_t *)malloc(needed * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        cache[3] = (seq_t)2;
        cache_len = 4;
    }

    /* if the requested term is already cached, return it */
    if (n < cache_len)
        return cache[n];

    /* extend cache if necessary */
    if (n + 1 > cache_len) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL)     /* allocation failed */
            return (seq_t)0;
        cache = tmp;
        /* compute new entries using the recurrence */
        for (idx_t idx = cache_len; idx <= n; ++idx) {
            seq_t a_prev = cache[idx - 1];
            seq_t a_n3   = cache[idx - 3];
            seq_t a_n4   = cache[idx - 4];

            /* factor (idx-1)*(idx-2) */
            seq_t num = (seq_t)(idx - 1) * (seq_t)(idx - 2);

            /* inner = 3*a_n3 + (idx-3)*a_n4 */
            seq_t inner = (seq_t)3 * a_n3 + (seq_t)(idx - 3) * a_n4;

            /* add = num * inner / 6 */
            seq_t add = (num * inner) / (seq_t)6;

            cache[idx] = a_prev + add;
        }
        cache_len = n + 1;   /* cache now holds indices 0..n */
    }

    return cache[n];
}

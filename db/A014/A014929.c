/*
 * A014929  a(1)=1, a(n) = n*14^(n-1) + a(n-1).
 * https://oeis.org/A014929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*14^(n-1) + a(n-1), a(1)=1 (OEIS A014929)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*                     ^ above is the implementing recurrence                */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cache   = NULL;   /* array of terms */
    static idx_t current = 0;       /* highest n currently computed         */
    static size_t cache_cap = 0;    /* allocated cache capacity             */
    static seq_t pow14 = 1;         /* 14^(current-1) , 14^0 when current==1 */

    if (n == 0) {                 /* undefined for n < 1 – return 0 */
        return 0;
    }

    /* Ensure the cache can hold at least n elements */
    if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 4;
        while (new_cap < (size_t)n)
            new_cap *= 2;
        /* Explicit cast to avoid C++ conversion error */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {                 /* allocation failed – return 0 */
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Extend the cache up to n */
    for (idx_t i = current + 1; i <= n; ++i) {
        if (i == 1) {
            cache[0] = 1;          /* a(1) = 1 */
            /* pow14 stays 14^0 = 1 */
        } else {
            pow14 = pow14 * 14;     /* now pow14 = 14^(i-1) */
            seq_t term = (seq_t)i * pow14;   /* i * 14^(i-1) */
            cache[i - 1] = cache[i - 2] + term;
        }
    }
    current = n;

    return cache[n - 1];
}

/*
 * A127210  a(n) = 3^n*Lucas(n), where Lucas = A000204.
 * https://oeis.org/A127210
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) + 9*a(n-2), a(1)=3, a(2)=27
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 56,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cap   = 0;   /* allocated size (>= last+1) */
    static idx_t last  = 0;   /* largest n for which cache[n] is valid */

    if (n < 1)
        return 0; /* behavior below offset is unspecified */

    /* Grow cache if needed */
    if (n >= cap) {
        idx_t new_cap = (cap == 0) ? n + 1 : cap;
        while (new_cap <= n)
            new_cap <<= 1;               /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failed */
            free(cache);
            cache = NULL;
            cap = 0;
            last = 0;
            return 0;
        }
        cache = tmp;
        cap = new_cap;
    }

    /* Compute any missing values up to n */
    for (idx_t k = last + 1; k <= n; ++k) {
        if (k == 1)
            cache[k] = (seq_t)3;
        else if (k == 2)
            cache[k] = (seq_t)27;
        else
            cache[k] = (seq_t)3 * cache[k - 1] + (seq_t)9 * cache[k - 2];
    }
    last = n;
    return cache[n];
}

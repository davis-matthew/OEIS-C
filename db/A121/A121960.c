/*
 * A121960  Expansion of g.f. x*(1 +30*x +49*x^2 -71*x^3
 *          -116*x^4)/((1+2*x)*(1-2*x^2) *(1-2*x-4*x^2)).
 * https://oeis.org/A121960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-2) + 8*a(n-3) - 16*a(n-4) - 16*a(n-5); Initial
 *            values: a(1)=1, a(2)=30, a(3)=59, a(4)=237, a(5)=698
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0; /* undefined; safe fallback */

    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static idx_t cached_terms = 0; /* number of terms currently stored */

    /* If needed, grow the cache to hold up to n terms (indices 0..n-1) */
    if (n > cached_terms) {
        size_t new_cap = n > cache_cap ? n : cache_cap;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failed */
        cache = tmp;
        cache_cap = new_cap;

        /* Compute missing terms */
        for (idx_t i = cached_terms; i < n; ++i) {
            seq_t val;
            if (i < 5) { /* initial values */
                switch (i) {
                    case 0: val = 1;         break;
                    case 1: val = 30;        break;
                    case 2: val = 59;        break;
                    case 3: val = 237;       break;
                    case 4: val = 698;       break;
                    default: val = 0;        /* never reached */
                }
            } else { /* recurrence */
                val = 10 * cache[i - 2]
                    + 8  * cache[i - 3]
                    - 16 * cache[i - 4]
                    - 16 * cache[i - 5];
            }
            cache[i] = val;
        }
        cached_terms = n;
    }
    return cache[n - 1];
}

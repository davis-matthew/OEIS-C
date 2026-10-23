/*
 * A192252  0-sequence of reduction of (n!) by x^2 -> x+1.
 * https://oeis.org/A192252
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the linear recurrence * a(n) = (n+1)*a(n-1) +
 *            n*(n-2)*a(n-2) - n*(n-1)*a(n-3) * with initial values a(0)=1,
 *            a(1)=1, a(2)=3
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: seq_t and idx_t are supplied by the build.  No other
 * dependencies are required.  */

#include <stdlib.h>   /* for malloc/realloc */

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;  /* array of a(k) */
    static idx_t  cached = 0;    /* number of terms currently cached (maximum index + 1) */

    /* Extend cache if necessary */
    if (n >= cached) {
        idx_t new_size = n + 1;              /* new required size */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;                  /* allocation failure – return 0 */
        cache = tmp;
        /* initialise missing terms */
        if (cached == 0) {          /* first time */
            cache[0] = (seq_t)1;    /* a(0) */
        }
        if (cached <= 1 && cached == 1) { /* we have a(0) only, need a(1) */
            cache[1] = (seq_t)1;    /* a(1) */
        }
        if (cached <= 2 && cached == 2) { /* we have a(0),a(1), need a(2) */
            cache[2] = (seq_t)3;    /* a(2) */
        }
        /* compute any remaining terms up to n */
        for (idx_t k = cached; k <= n; ++k) {
            if (k == 0) {
                cache[0] = (seq_t)1;
            } else if (k == 1) {
                cache[1] = (seq_t)1;
            } else if (k == 2) {
                cache[2] = (seq_t)3;
            } else {
                seq_t term1 = ((seq_t)(k + 1)) * cache[k - 1];
                seq_t term2 = (seq_t)k * (seq_t)(k - 2) * cache[k - 2];
                seq_t term3 = (seq_t)k * (seq_t)(k - 1) * cache[k - 3];
                cache[k] = term1 + term2 - term3;
            }
        }
        cached = new_size;
    }

    return cache[n];
}

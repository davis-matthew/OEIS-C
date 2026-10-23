/*
 * A101912  G.f. satisfies: A(x) = 1/(1 + x*A(x^2)) and also the continued
 *          fraction: 1 + x*A(x^3) = [1; 1/x, 1/x^2, 1/x^4, 1/x^8, ...,
 *          1/x^(2^(n-1)), ...].
 * https://oeis.org/A101912
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0) = 1; for n ≥ 1, a(n) = –∑_{k=0}^{⌊(n‑1)/2⌋} a(k) * a(n – 2k
 *            – 1) (OEIS A101912)
 * Range:     i8 n <= 26, i16 n <= 48, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* stored sequence values */
    static idx_t used   = 0;      /* cache size (number of elements) */

    if (n >= used) {
        /* enlarge cache to hold up to n */
        idx_t newsize = n + 1;
        void *p = realloc(cache, newsize * sizeof(seq_t));
        if (!p) return 0;        /* allocation failed – return 0 as a fallback */
        cache = (seq_t *)p;

        if (used == 0) {
            cache[0] = (seq_t)1;   /* a(0) = 1 */
            used = 1;
        }

        /* compute from the first missing index up to n */
        for (idx_t i = used; i <= n; ++i) {
            if (i == 0) continue;   /* already set */

            seq_t sum = (seq_t)0;
            idx_t limit = (i - 1) / 2;
            for (idx_t k = 0; k <= limit; ++k) {
                sum += cache[k] * cache[i - 2*k - 1];
            }
            cache[i] = -sum;
        }
        used = n + 1;   /* update cache size */
    }

    return cache[n];
}

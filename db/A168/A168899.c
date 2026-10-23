/*
 * A168899  Number of reduced words of length n in Coxeter group on 30
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^21 = I.
 * https://oeis.org/A168899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p[n] + 28*∑_{i=1}^{min(20,n)} a(n-i) – 406*(n≥21?
 *            a(n-21):0) where p[0] = 1, p[1..20] = 2, p[21] = 1
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 13, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* coefficients of the numerator of the generating function */
    static const seq_t p[22] = {
        1, /* n = 0 */
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2,
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, /* n = 1..20 */
        1  /* n = 21 */
    };

    /* cache of previously computed terms */
    static seq_t *cache   = NULL;
    static size_t   cap   = 0;  /* allocated size (in elements)   */
    static size_t   len   = 0;  /* number of terms stored (indices 0..len‑1) */

    /* ensure the cache is large enough to hold terms up to n */
    if (n >= (idx_t)cap) {
        size_t new_cap = cap ? cap << 1 : 16;
        if (new_cap < (size_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return 0;          /* allocation failed – return 0 */
        cache = tmp;
        cap   = new_cap;
    }

    /* compute missing terms up to n */
    for (size_t i = len; i <= (size_t)n; ++i) {
        seq_t sum = p[i];                    /* coefficient of t^i in numerator */
        size_t kmax = i < 20 ? i : 20;      /* how many previous terms to use   */

        for (size_t k = 1; k <= kmax; ++k) {
            sum += 28 * cache[i - k];
        }

        if (i >= 21) {
            sum -= 406 * cache[i - 21];
        }

        cache[i] = sum;
    }
    len = n + 1;

    return cache[n];
}

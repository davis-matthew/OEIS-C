/*
 * A061700  Generalized Bell numbers.
 * https://oeis.org/A061700
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=1; a(1)=a(2)=0; for n≥3, * a(n) = (1/n) * Σ_{k=3}^{n} k *
 *            (C(n,k))^3 * a(n-k) * where C(n,k) is the binomial coefficient
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached a(0)…a(cache_len‑1) */
    static idx_t cache_len = 0;

    /* initialise cache with a(0)=1 */
    if (cache == NULL) {
        cache_len = 1;
        cache = (seq_t *)calloc(cache_len, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;
    }

    /* extend cache up to n, if necessary */
    if (n >= cache_len) {
        /* reallocate to store a(0)…a(n) */
        seq_t *tmp = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;     /* allocation failure */
        /* zero any newly allocated entries */
        memset(tmp + cache_len, 0, (n + 1 - cache_len) * sizeof(seq_t));
        cache = tmp;

        /* compute all new values in increasing order */
        for (idx_t m = cache_len; m <= n; ++m) {
            if (m < 3) {        /* a(1)=a(2)=0 by the recurrence */
                cache[m] = (seq_t)0;
                continue;
            }

            seq_t sum = (seq_t)0;
            seq_t c   = (seq_t)1;  /* C(m,0) */

            for (idx_t k = 1; k <= m; ++k) {
                /* update C(m,k) from C(m,k‑1) */
                c = (c * (seq_t)(m - k + 1)) / (seq_t)k;

                if (k >= 3) {
                    seq_t c2 = c * c;          /* C(m,k)^2 */
                    seq_t c3 = c2 * c;         /* C(m,k)^3 */

                    seq_t term = cache[m - k];  /* a(m‑k) */
                    term = term * c3;           /* * (C(m,k))^3 */
                    term = term * (seq_t)k;     /* * k */

                    sum += term;
                }
            }

            /* division by m is exact */
            cache[m] = sum / (seq_t)m;
        }

        cache_len = n + 1;
    }

    return cache[n];
}

/*
 * A010741  Shifts 3 places left under inverse binomial transform.
 * https://oeis.org/A010741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(0)=1, a(1)=2, a(2)=4, a(3)=1, for n≥4 a(n) = S(n) – a(n-1),
 *            where S(n) = Σ_{k=1}^{n‑3} (-1)^{n‑k‑3} · C(n‑4, k‑1) · a(k)
 * Range:     i8 n <= 10, i16 n <= 15, i32 n <= 24, i64 n <= 36, i128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* External integer types provided by the build system */
#include <stdlib.h>

/* Implement the sequence function */
seq_t a(idx_t n)
{
    /* Static cache for previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n + 1 > cache_size) {
        /* Grow the cache */
        size_t new_size = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache = tmp;

        /* Compute terms up to n */
        for (idx_t m = cache_size; m <= n; ++m) {
            switch (m) {
                case 0: cache[0] = (seq_t)1; break;
                case 1: cache[1] = (seq_t)2; break;
                case 2: cache[2] = (seq_t)4; break;
                case 3: cache[3] = (seq_t)1; break;
                default:
                {
                    seq_t sum = (seq_t)0;
                    seq_t binom = (seq_t)1;   /* C(m-4,0) */
                    idx_t upper = m - 3;

                    for (idx_t k = (idx_t)1; k <= upper; ++k) {
                        /* sign = (-1)^(m-k-3) */
                        if ( ((m - k - 3) & (idx_t)1) == 0 )
                            sum += cache[(size_t)k] * binom;
                        else
                            sum -= cache[(size_t)k] * binom;

                        /* Update binomial coefficient to C(m-4, k) for next k */
                        if (k < upper) {
                            seq_t numer = (seq_t)(m - 4 - (k - 1));
                            binom = binom * numer / (seq_t)k;
                        }
                    }
                    cache[(size_t)m] = sum - cache[(size_t)(m - 1)];
                }
            }
        }
        cache_size = new_size;
    }
    return cache[(size_t)n];
}

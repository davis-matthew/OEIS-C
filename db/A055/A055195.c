/*
 * A055195  Number of 3-covers of an unlabeled n-set.
 * https://oeis.org/A055195
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) satisfies a linear recurrence of order 13 (see the
 *            generating function in the OEIS entry). For all n ≥ 2 a(n) is
 *            given by a(n) = 3 a(n‑1) – a(n‑2) – 3 a(n‑3) – a(n‑4) + 3 a(n‑5)
 *            + 6 a(n‑6) – 6 a(n‑7) – 3 a(n‑8) + a(n‑9) + 3 a(n‑10) + a(n‑11)
 *            – 3 a(n‑12) + a(n‑13) + c(n), with the boundary co...
 * Range:     u8 n <= 6, u16 n <= 22, u32 n <= 159, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdlib.h>   /* for malloc, realloc, abort */

/* recurrence coefficients r[k] = coefficient of a(n-k), 1 ≤ k ≤ 13 */
static const int r_coef[14] = {
    0,   /* unused */
    3,  -1, -3, -1,  3,  6, -6, -3,  1,  3,  1, -3,  1
};

/* boundary correction constants c[n] for n = 0 … 13 */
static const int c_coef[14] = {
    0, 0, 1, 6, 3, -3, -5, -3, 6, 4, 0, -3, -1, 1
};

/* cache of computed a(n); cache[0] = a(0), cache[1] = a(1), ... */
static seq_t *cache = NULL;
static idx_t cache_len = 0;     /* number of elements stored (cache_len ≥ 2 once initialized) */

/* compute a(n) for any n ≥ 2, memoized in static cache */
seq_t a(idx_t n)
{
    if (n < 2) return (seq_t)0;   /* undefined indices are defined to be 0 */

    /* grow the cache if necessary */
    if (cache_len <= n) {
        idx_t new_len = n + 1;
        seq_t *newc = NULL;
        if (cache_len == 0)
            newc = (seq_t *)malloc(new_len * sizeof(seq_t));
        else
            newc = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!newc) abort();
        cache = newc;
        /* initialise first two entries (a(0) and a(1)) */
        if (cache_len == 0) {
            cache[0] = (seq_t)0;
            cache[1] = (seq_t)0;
        }
        /* compute terms from cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = (seq_t)0;
            for (int k = 1; k <= 13; ++k) {
                if (i >= (idx_t)k) {
                    sum += cache[i - k] * (seq_t)r_coef[k];
                }
            }
            if (i < 14) sum += (seq_t)c_coef[i];
            cache[i] = sum;
        }
        cache_len = new_len;
    }
    return cache[n];
}

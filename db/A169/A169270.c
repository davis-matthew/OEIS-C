/*
 * A169270  Number of reduced words of length n in Coxeter group on 17
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^29 = I.
 * https://oeis.org/A169270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence derived from the generating function:
 *
 *   a(n) = b(n) + 15 * Σ_{k=1}^{28} a(n-k) – 120 * a(n-29),
 *        where   a(k) = 0 for k < 0,
 *                b(0) = 1,
 *                b(n) = 2 for 1 ≤ n ≤ 28,
 *                b(29) = 1,
 *                b(n) = 0 for n ≥ 30.
 *
 * This implements a memoised, iterative calculation of any a(n) for n ≥ 0.
 * The function is safe for arbitrary precision seq_t and scans up to the
 * required index without recomputing earlier terms.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;   /* number of computed elements */
    static idx_t  cache_cap   = 0;   /* allocated capacity */

    /* Initial allocation or grow if necessary.  Cast the result of calloc/
       realloc because the build might be in C++. */
    if (cache == NULL) {
        cache_cap = (n < 64 ? 64 : n + 1);
        cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        if (cache == NULL) abort();
        cache_size = 0;
    } else if ((n + 1) > cache_cap) {
        cache_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;
    }

    /* Compute any missing terms up to index n. */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;

        /* Σ_{k=1}^{28} a(i-k) (terms < 0 are zero). */
        idx_t maxk = (i < 28 ? i : 28);
        for (idx_t k = 1; k <= maxk; ++k) {
            sum += cache[i - k];
        }

        seq_t val = sum * (seq_t)15;
        if (i >= 29) {
            val -= cache[i - 29] * (seq_t)120;
        }

        /* Add b(i). */
        if (i == 0) {
            val += (seq_t)1;
        } else if (i <= 28) {
            val += (seq_t)2;
        } else if (i == 29) {
            val += (seq_t)1;
        }

        cache[i] = val;
    }

    cache_size = n + 1;   /* whole range [0..n] is now valid */
    return cache[n];
}

/*
 * A008440  Expansion of Jacobi theta constant theta_2^6 /(64q^(3/2)).
 * https://oeis.org/A008440
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in (∑_{m≥0} x^{m(m+1)/2})^6 The
 *            routine computes the number of representations of n as a sum of
 *            six triangular numbers
 * Range:     u8 n <= 10, u16 n <= 177, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache of previously computed values */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;  /* cache_size = max(n)+1 */

    /* Return cached value if already known */
    if (n < cache_size) return cache[n];

    idx_t N = n;

    /* Temporary arrays for the six convolutions */
    seq_t *dp_prev;
    seq_t *dp_curr;
#ifdef __cplusplus
    dp_prev = new seq_t[N + 1](); /* value-initialised to zero */
    dp_curr = new seq_t[N + 1]();
#else
    dp_prev = (seq_t *)calloc(N + 1, sizeof(seq_t));
    dp_curr = (seq_t *)calloc(N + 1, sizeof(seq_t));
#endif

    /* Start with exactly one way to make 0 */
    dp_prev[0] = 1;

    /* Perform six successive convolutions with the triangular series */
    for (idx_t k = 0; k < 6; ++k) {
        /* Clear dp_curr */
        for (idx_t i = 0; i <= N; ++i) dp_curr[i] = 0;

        /* Loop over triangular numbers t <= N */
        for (idx_t m = 0; ; ++m) {
            seq_t t_seq = (seq_t)m * ((seq_t)m + 1) / 2;
            if (t_seq > (seq_t)N) break;
            idx_t t = (idx_t)t_seq;

            for (idx_t s = t; s <= N; ++s) {
                dp_curr[s] += dp_prev[s - t];
            }
        }
        /* Swap for the next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_curr;
        dp_curr = tmp;
    }

    /* Resize (or create) the cache to hold results up to N */
#ifdef __cplusplus
    if (cache == NULL) {
        cache = new seq_t[N + 1]();
    } else if (cache_size <= N) {
        seq_t *new_cache = new seq_t[N + 1]();
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        delete[] cache;
        cache = new_cache;
    }
#else
    if (cache == NULL) {
        cache = (seq_t *)calloc(N + 1, sizeof(seq_t));
    } else if (cache_size <= N) {
        seq_t *new_cache = (seq_t *)calloc(N + 1, sizeof(seq_t));
        for (idx_t i = 0; i < cache_size; ++i) new_cache[i] = cache[i];
        free(cache);
        cache = new_cache;
    }
#endif

    /* Copy the freshly computed values into the cache */
    for (idx_t i = 0; i <= N; ++i) cache[i] = dp_prev[i];
    cache_size = N + 1;

    /* Free the temporary DP arrays */
#ifdef __cplusplus
    delete[] dp_prev;
    delete[] dp_curr;
#else
    free(dp_prev);
    free(dp_curr);
#endif

    return cache[n];
}

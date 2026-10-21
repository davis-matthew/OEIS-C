/*
 * A202194  Number of (n+2)X(n+2) binary arrays avoiding patterns 001 and 101
 *          in rows and columns.
 * https://oeis.org/A202194
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = 4*(n+2)^2 * binom(2*n+1, n)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 12, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>

seq_t a(idx_t n) {
    static seq_t *B_cache = NULL;   /* binom(2*k+1, k) for k<=cache_size-1 */
    static seq_t *A_cache = NULL;   /* a(k) values */
    static size_t cache_size = 0;   /* current cache capacity */

    if (n >= cache_size) {          /* extend cache to include n */
        size_t new_size = (size_t)n + 1;
        seq_t *new_B = (seq_t *)malloc(new_size * sizeof(seq_t));
        seq_t *new_A = (seq_t *)malloc(new_size * sizeof(seq_t));
        if (!new_B || !new_A) abort();

        /* copy old values */
        if (B_cache) {
            memcpy(new_B, B_cache, cache_size * sizeof(seq_t));
            free(B_cache);
        }
        if (A_cache) {
            memcpy(new_A, A_cache, cache_size * sizeof(seq_t));
            free(A_cache);
        }
        B_cache = new_B;
        A_cache = new_A;

        /* compute new entries */
        for (size_t k = cache_size; k <= n; ++k) {
            if (k == 0) {               /* unused sentinel for k=0 */
                B_cache[0] = (seq_t)1;
                A_cache[0] = (seq_t)0;
                continue;
            }
            idx_t kk = (idx_t)k;
            seq_t B_prev = B_cache[k - 1];

            /* B_k = B_{k-1} * (2k+1)(2k) / (k(k+1)) */
            seq_t numerator = B_prev * (seq_t)(2 * kk + 1) * (seq_t)(2 * kk);
            seq_t denom     = (seq_t)kk * (seq_t)(kk + 1);
            seq_t B_cur     = numerator / denom;
            B_cache[k] = B_cur;

            /* a_k = 4*(k+2)^2 * B_k */
            seq_t k2     = (seq_t)(kk + 2);
            seq_t factor = (seq_t)4 * k2 * k2;
            seq_t a_val  = factor * B_cur;
            A_cache[k] = a_val;
        }
        cache_size = new_size;
    }
    return A_cache[n];
}

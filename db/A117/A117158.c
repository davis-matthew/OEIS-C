/*
 * A117158  Number of permutations of [n] avoiding the consecutive pattern
 *          1234.
 * https://oeis.org/A117158
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: the number of permutations of [n] avoiding the consecutive
 * pattern 1234.  The exponential generating function is
 *   A(x) = 2/(e^(-x) + cos x - sin x)
 * The recurrence below is derived from the relation
 *   (e^(-x) + cos x - sin x) * A(x) = 2
 * which yields for n >= 1:
 *   a[n] = Σ_{k=0}^{n-1} binom(n,k) * (C_{n-k}/2) * a[k]
 * with C_m = 2 if m ≡ 0 (mod 4), -2 if m ≡ 1 (mod 4), 0 otherwise.
 * To avoid negative intermediate values we accumulate the positive
 * contributions (m ≡ 0) and negative contributions (m ≡ 1) separately
 * and return their difference.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of elements allocated (>=1) */

    /* Ensure the cache is large enough */
    if (n + 1 > cache_len) {
        size_t new_len = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!new_cache) {
            /* Out of memory – terminate silently (should not happen). */
            abort();
        }
        cache = new_cache;
        cache_len = (idx_t)new_len;

        /* Initialize the new entries */
        if (cache_len >= 1) cache[0] = (seq_t)1;          /* a(0) = 1 */
        if (cache_len >= 2) cache[1] = (seq_t)1;          /* a(1) = 1 */
        if (n < 2) return cache[n];

        /* Compute new values up to n */
        for (idx_t k = 2; k <= n; ++k) {
            seq_t pos_sum = 0;   /* sum for m ≡ 0 (mod 4) */
            seq_t neg_sum = 0;   /* sum for m ≡ 1 (mod 4) */
            seq_t bin = 1;       /* binom(k,0) */

            /* Inner loop over previous indices j = 0 .. k-1 */
            for (idx_t j = 0; j < k; ++j) {
                idx_t m = k - j;     /* m = k - j */
                int sign = 0;
                if (m % 4 == 0) sign = 1;
                else if (m % 4 == 1) sign = -1;

                if (sign != 0) {
                    seq_t prod = bin * cache[j];
                    if (sign > 0) {
                        pos_sum += prod;
                    } else {
                        neg_sum += prod;
                    }
                }

                /* Update binom(k, j+1) for next j */
                if (j + 1 < k) {
                    bin = bin * (seq_t)(k - j);
                    bin = bin / (seq_t)(j + 1);
                }
            }

            /* a[k] = neg_sum - pos_sum  (neg_sum >= pos_sum) */
            cache[k] = neg_sum;
            cache[k] -= pos_sum;
        }
    }

    return cache[n];
}

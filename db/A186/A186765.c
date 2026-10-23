/*
 * A186765  Number of permutations of {1,2,...,n} having no increasing even
 *          cycles. A cycle (b(1), b(2), ...) is said to be increasing if,
 *          when written with its smallest element in the first position, it
 *          satisfies b(1)<b(2)<b(3)<... . A cycle is said to be even if it
 *          has an even number of entries.
 * https://oeis.org/A186765
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

/* Recurrence (for n ≥ 1):
   a(0) = 1
   a(n) = n*a(n-1)
          – Σ_{odd j=1}^{n-1} a(n-1-j)*C(n-1,j)
          + (n-1)*Σ_{odd j=1}^{n-2} a(n-2-j)*C(n-2,j)
   where C(a,b) is the binomial coefficient.  The coefficients
   are computed iteratively using the identity
   C(a,j) = C(a,j-1)*(a-j+1)/j, guaranteeing integer arithmetic. */
#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of stored elements (≥1 after init) */

    /* initialise cache if necessary */
    if (cache_len == 0) {
        /* a(0) = 1 */
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return (seq_t)0;  /* allocation failed */
        cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* extend cache if needed */
    if (cache_len <= n) {
        /* remember old length before reallocation */
        idx_t old_len = cache_len;
        /* reallocate to hold indices 0…n */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;  /* allocation failed */
        cache = tmp;
        cache_len = (idx_t)(n + 1);

        /* compute new values from old_len up to n */
        for (idx_t k = old_len; k <= n; ++k) {
            if (k == 0) {  /* already initialised */
                cache[0] = (seq_t)1;
                continue;
            }

            /* --- compute sum1: Σ_{odd j=1}^{k-1} a(k-1-j)*C(k-1,j) --- */
            seq_t sum1 = (seq_t)0;
            seq_t binom = (seq_t)1;   /* C(k-1,0) */
            for (idx_t j = 1; j < k; ++j) {
                binom = binom * (seq_t)(k - j); /* binom *= (k-j) */
                binom = binom / (seq_t)j;       /* exact division */
                if (j & 1UL) {                 /* j odd */
                    sum1 += cache[k - 1 - j] * binom;
                }
            }

            /* --- compute sum2: Σ_{odd j=1}^{k-2} a(k-2-j)*C(k-2,j) --- */
            seq_t sum2 = (seq_t)0;
            if (k >= 2) {
                seq_t binom2 = (seq_t)1;       /* C(k-2,0) */
                for (idx_t j = 1; j < k - 1; ++j) {
                    binom2 = binom2 * (seq_t)(k - 1 - j); /* binom2 *= (k-1-j) */
                    binom2 = binom2 / (seq_t)j;           /* exact division */
                    if (j & 1UL) {                       /* j odd */
                        sum2 += cache[k - 2 - j] * binom2;
                    }
                }
            }

            /* --- combine to obtain a(k) --- */
            seq_t tmp = (seq_t)k * cache[k - 1];
            if (k >= 2) {
                tmp += (seq_t)(k - 1) * sum2;
            }
            cache[k] = tmp - sum1;                /* tmp ≥ sum1, no underflow */
        }
    }

    return cache[n];
}

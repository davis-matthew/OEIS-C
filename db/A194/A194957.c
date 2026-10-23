/*
 * A194957  E.g.f. satisfies: A(x) = cos(x) + x*A(x)^2.
 * https://oeis.org/A194957
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (-1)^{n/2} + n * Σ_{k=0}^{n-1} binomial(n-1,k) * a(k) *
 *            a(n-1-k) a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute sequence A194957.  The implementation uses memoization
   and a symmetry‑reduced convolution of the recurrence formula.  */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_len = 0;

seq_t a(idx_t n) {
    /* Ensure cache length suffices */
    if (n >= cache_len) {
        size_t new_len = cache_len ? cache_len * 2 : 1;
        if (new_len < (size_t)n + 1) new_len = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) abort();   /* memory exhausted */
        cache = tmp;
        /* Initialize newly allocated elements to zero (optional) */
        for (size_t i = cache_len; i < new_len; ++i) cache[i] = 0;
        /* Fill in terms up to n */
        for (idx_t m = (idx_t)cache_len; m <= n; ++m) {
            if (m == 0) {
                cache[0] = 1;
                continue;
            }

            idx_t k = m - 1;                 /* convolution length */
            seq_t binom = 1;                 /* binomial(k,0) */
            seq_t sum   = 0;

            /* Convolution with symmetry */
            for (idx_t i = 0; i <= k / 2; ++i) {
                idx_t j = k - i;
                seq_t prod = cache[i] * cache[j];
                prod = prod * binom;
                if (i < j) prod <<= 1;        /* multiply by 2 for the pair */
                sum += prod;

                /* Update binomial for next i, unless last iteration */
                if (i < k / 2) {
                    seq_t mult = (seq_t)(k - i);   /* (k - i) */
                    binom = binom * mult;
                    binom = binom / ((seq_t)(i + 1));
                }
            }

            seq_t term = sum * ((seq_t)m);       /* multiply by m */

            /* Add the cosine correction for even n */
            if ((m & 1) == 0) {                 /* m even */
                seq_t half = m >> 1;
                if ((half & 1) == 1) {          /* (-1)^ (n/2) = -1 */
                    term -= 1;
                } else {                        /* (+1) */
                    term += 1;
                }
            }

            cache[m] = term;
        }
        cache_len = (size_t)n + 1;
    }
    return cache[(size_t)n];
}

/*
 * A170216  Number of reduced words of length n in Coxeter group on 15
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170216
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the number of reduced words of length n in the Coxeter
 *            group on 15 generators with relations (S_i)^2 = (S_i S_j)^40 =
 *            I. Generating function: G(t) = (1 + 2*(t + … + t^39) + t^40) /
 *            (1 - 13*(t + … + t^39) + 91*t^40). The identity (1 - 13*t - …
 *            -13*t^39 + 91*t^40) * G(t) = 1 + 2*t + … + 2*t...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mul_small.h"

static seq_t *cache      = NULL;  /* cached sequence values   */
static size_t cache_cap  = 0;     /* allocated capacity       */
static idx_t  cache_len  = 0;     /* number of values stored  */
static seq_t  sum_all    = 0;     /* sum of all stored terms  */

seq_t a(idx_t n)
{
    /* Grow the cache if necessary */
    if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!tmp) return 0;                 /* allocation failure */
        if (cache) {
            memcpy(tmp, cache, cache_len * sizeof(seq_t));
            free(cache);
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;
        } else if (i <= 39) {
            /* a(i) = 2 + 13*sum_{k=0}^{i-1} a(k)  (sum_all holds that sum) */
            val = 2 + mul_small(sum_all, 13);
        } else {
            /* a(i) = 13*Σ_{k=1}^{39} a(i-k) – 91*a(i-40) */
            seq_t sum_prev39 = 0;
            for (idx_t j = 1; j <= 39; ++j) sum_prev39 += cache[i - j];
            seq_t term = cache[i - 40];
            seq_t t1 = mul_small(sum_prev39, 13);
            seq_t t2 = mul_small(term, 91);
            val = t1 - t2;
        }
        cache[i] = val;
        sum_all += val;
    }

    cache_len = n + 1;   /* update stored length */
    return cache[n];
}

/*
 * A042899  Denominators of continued fraction convergents to sqrt(981).
 * https://oeis.org/A042899
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   denominators of continued fraction convergents to sqrt(981)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 19, u64 n <= 42, u128 n <= 79,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/* Continued‑fraction coefficients for sqrt(981), excluding the first term a0 = 31.
   The period has length 30. */
static const seq_t PERIOD[30] = {
    (seq_t)3,  (seq_t)8,  (seq_t)1,  (seq_t)1,
    (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)1,
    (seq_t)2,  (seq_t)1,  (seq_t)1,  (seq_t)15,
    (seq_t)12, (seq_t)2,  (seq_t)6,  (seq_t)2,
    (seq_t)12, (seq_t)15, (seq_t)1,  (seq_t)1,
    (seq_t)2,  (seq_t)1,  (seq_t)1,  (seq_t)1,
    (seq_t)1,  (seq_t)1,  (seq_t)1,  (seq_t)8,
    (seq_t)3,  (seq_t)62
};

static seq_t *cache        = NULL;
static idx_t  cache_len    = 0;
static size_t cache_cap    = 0;

/* a_0 = 31, a_k for k >= 1 comes from PERIOD. */
static inline seq_t period_term(idx_t idx)
{
    /* idx >= 1 */
    return PERIOD[(idx - 1) % 30];
}

seq_t a(idx_t n)
{
    /* Ensure the cache contains at least a(0). */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(sizeof(seq_t));
        if (!cache) return 0;        /* out of memory */
        cache_cap = 1;
        cache[0]   = (seq_t)1;         /* q_0 = 1   */
        cache_len  = 1;
    }

    /* Grow the cache if needed. */
    if ((idx_t)(n + 1) > cache_len) {
        idx_t need = (idx_t)(n + 1);

        if ((size_t)need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 1;
            while (new_cap < (size_t)need) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) return 0;        /* out of memory */
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms. */
        for (idx_t k = cache_len; k <= n; ++k) {
            seq_t a_k = (k == 0) ? (seq_t)31 : period_term(k);
            seq_t q_prev1 = cache[k - 1];
            seq_t q_prev2 = (k >= 2) ? cache[k - 2] : (seq_t)0;
            cache[k] = a_k * q_prev1 + q_prev2;
        }
        cache_len = n + 1;
    }

    return cache[n];
}

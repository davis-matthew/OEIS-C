/*
 * A197058  Number of n X 5 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors equal to 2.
 * https://oeis.org/A197058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 60, u128 n <= 121,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A197058: Number of n × 5 0..4 arrays with each element equal to the number of its horizontal
 * and vertical neighbors equal to 2.  Empirical linear recurrence of order 13:
 *   a(n) = 2·a(n‑1) – a(n‑2) + a(n‑3) + 2·a(n‑4) + 3·a(n‑5) – a(n‑6)
 *          – 2·a(n‑7) – 2·a(n‑8) + 2·a(n‑9) – 2·a(n‑10) – a(n‑11) – 3·a(n‑12) – a(n‑13)
 *   with initial terms:
 *        a(1)=1, a(2)=5, a(3)=14, a(4)=28, a(5)=48, a(6)=90, a(7)=200,
 *        a(8)=447, a(9)=937, a(10)=1881, a(11)=3813, a(12)=7947, a(13)=16720.
 */

#include <stdlib.h>

/* Initial 13 terms of the sequence.  These are needed to seed the recurrence. */
static const seq_t init_terms[13] = {
    (seq_t)1,  (seq_t)5,  (seq_t)14, (seq_t)28, (seq_t)48,
    (seq_t)90, (seq_t)200, (seq_t)447, (seq_t)937, (seq_t)1881,
    (seq_t)3813, (seq_t)7947, (seq_t)16720
};

/* Coefficients of the recurrence a(n)= Σ coeff[i]*a(n-i-1), i=0..12. */
static const signed int coeffs[13] = {
    2, -1, 1, 2, 3, -1, -2, -2, 2, -2, -1, -3, -1
};

seq_t a(idx_t n)
{
    /* Cache all previously computed terms (a(1) … a(k)).  The cache is grown lazily. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of terms currently stored (≥0) */
    static size_t cache_cap = 0;   /* allocated capacity (≥0) */

    /* Smallest n we can handle is 1.  If n<1, behavior is undefined; we simply return 0. */
    if (n < 1) return (seq_t)0;

    /* Ensure capacity for n terms.  A small safety margin is added to reduce reallocs. */
    if (cache_cap < (size_t)n) {
        size_t new_cap = n + 13;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* out‑of‑memory: return 0 */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the cache with the first 13 terms if not already done. */
    if (cache_len < 13) {
        size_t limit = 13u;
        if (cache_len < limit) {
            for (size_t i = cache_len; i < limit; ++i) {
                cache[i] = init_terms[i];
            }
            cache_len = limit;
        }
    }

    /* Compute missing terms up to n. */
    for (idx_t i = (idx_t)(cache_len + 1); i <= n; ++i) {
        /* For i <= 13, the value is already stored; skip staying safe. */
        if (i <= 13) continue;

        seq_t val = (seq_t)0;
        for (int k = 1; k <= 13; ++k) {
            /* a(i-k) is stored at cache[i-k-1] (0‑based index). */
            val += (seq_t)(coeffs[k - 1]) * cache[(size_t)(i - k - 1)];
        }
        cache[(size_t)(i - 1)] = val;
    }

    cache_len = (size_t)n;
    return cache[(size_t)(n - 1)];
}

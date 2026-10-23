/*
 * A195660  Number of ways to place 11n nonattacking kings on a vertical
 *          cylinder 22 X 2n.
 * https://oeis.org/A195660
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 1, u32 n <= 5, u64 n <= 28, u128 n <= 89,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = –4a(n‑12) + 44a(n‑11) – 221a(n‑10) + 670a(n‑9) – 1365a(n‑8)
 *             + 1968a(n‑7) – 2058a(n‑6) + 1572a(n‑5) – 870a(n‑4) + 340a(n‑3)
 *             – 89a(n‑2) + 14a(n‑1)
 * Initial values (a(1)…a(12)):
 * 4096, 433500, 11682296, 153802520, 1301236304, 8155899320, 41180193352,
 * 176740657340, 668845118276, 2290966142762, 7241521734020, 21437333168798.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

/* Initial values of a(1) … a(12) */
static const seq_t init_vals[12] = {
    (seq_t)4096ULL,
    (seq_t)433500ULL,
    (seq_t)11682296ULL,
    (seq_t)153802520ULL,
    (seq_t)1301236304ULL,
    (seq_t)8155899320ULL,
    (seq_t)41180193352ULL,
    (seq_t)176740657340ULL,
    (seq_t)668845118276ULL,
    (seq_t)2290966142762ULL,
    (seq_t)7241521734020ULL,
    (seq_t)21437333168798ULL
};

/* Recurrence coefficients from a(n‑1) to a(n‑12) */
static const int coeff[12] = {
    14, -89, 340, -870, 1572, -2058, 1968,
    -1365, 670, -221, 44, -4
};

/* Cache for previously computed terms */
static seq_t *cache       = NULL;
static size_t cache_cap   = 0;   /* allocated capacity */
static size_t cache_len   = 0;   /* number of values stored */

/* Ensure that the cache has capacity for at least n elements (1‑based length). */
static void ensure_capacity(size_t n)
{
    if (n <= cache_cap)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 1;
    while (new_cap < n)
        new_cap *= 2;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache     = tmp;
    cache_cap = new_cap;
}

/* Compute a(n) for n >= 1 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;            /* undefined, return 0 */

    /* First call: initialise cache with known values. */
    if (cache_len == 0) {
        ensure_capacity(12);
        for (size_t i = 0; i < 12; ++i)
            cache[i] = init_vals[i];
        cache_len = 12;
    }

    /* Compute further terms if required. */
    if ((size_t)n > cache_len) {
        ensure_capacity((size_t)n);
        for (size_t idx = cache_len; idx < (size_t)n; ++idx) {
            seq_t sum_pos = 0;
            seq_t sum_neg = 0;
            for (int j = 1; j <= 12; ++j) {
                size_t prev_idx = idx - j;           /* a(idx+1 - j) is at cache[prev_idx] */
                seq_t val = cache[prev_idx];
                int c = coeff[j - 1];
                if (c >= 0)
                    sum_pos += (seq_t)c * val;
                else
                    sum_neg += (seq_t)(-c) * val;
            }
            /* a(idx+1) = sum_pos - sum_neg  (pos_sum >= neg_sum for all n). */
            cache[idx] = sum_pos - sum_neg;
        }
        cache_len = (size_t)n;
    }

    return cache[n - 1];
}

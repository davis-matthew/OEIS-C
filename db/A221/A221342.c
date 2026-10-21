/*
 * A221342  Number of nX3 arrays of occupancy after each element stays put or
 *          moves to some horizontal, diagonal or antidiagonal neighbor, with
 *          no occupancy greater than 2.
 * https://oeis.org/A221342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 7. a(n) =
 *            43*a(n-1)-604*a(n-2)+3305*a(n-3)-5534*a(n-4)-8103*a(n-5)
 *            +18838*a(n-6)+17434*a(n-7)-4723*a(n-8)-11537*a(n-9)-7289*a(n-10)
 *            -2900*a(n-11)-652*a(n-12)-296*a(n-13)-94*a(n-14)-16*a(n-15);
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   seq_from_str
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/seq_from_str.h"

seq_t a(idx_t n)
{
    /* 1‑based cache */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static bool init_done = false;

    /* String representations of the first 15 terms (index 0 unused) */
    static const char *init_str[16] = {
        NULL,
        "7", "137", "2856", "60807", "1299057", "27767394", "593562702",
        "12688126159", "271223239234", "5797699341153",
        "123932233563380", "2649188126866350", "56629314186026164",
        "1210513943745878094", "25876068208481143920"
    };

    /* Allocate cache for the first 15 values if not yet done */
    if (!init_done) {
        /* Ensure cache capacity up to index 15 (include 0) */
        size_t needed = 16;
        cache = (seq_t *)realloc(cache, needed * sizeof(seq_t));
        cache_size = needed;
        for (idx_t i = 1; i <= 15; ++i) {
            cache[i] = seq_from_str(init_str[i]);
        }
        init_done = true;
    }

    /* Return directly if n is within the initial range */
    if (n <= 15) {
        return cache[(size_t)n];
    }

    /* Ensure the cache can hold up to index n (1‑based) */
    if (n + 1u > cache_size) {
        size_t new_cap = n + 1u;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_size = new_cap;
    }

    /* Recurrence coefficients (positive first, negative second) */
    static const int coeff_pos[4]  = {43, 3305, 18838, 17434};
    static const int coeff_idx_pos[4] = {1, 3, 6, 7};
    static const int coeff_neg[11]  = {604, 5534, 8103, 4723, 11537,
                                       7289, 2900, 652, 296, 94, 16};
    static const int coeff_idx_neg[11] = {2, 4, 5, 8, 9, 10, 11, 12, 13, 14, 15};

    /* Compute terms from 16 up to n */
    for (idx_t k = 16; k <= n; ++k) {
        seq_t pos_sum = 0;
        seq_t neg_sum = 0;

        /* Positive contributions */
        for (int i = 0; i < 4; ++i) {
            pos_sum += cache[(size_t)(k - coeff_idx_pos[i])] *
                       (seq_t)coeff_pos[i];
        }

        /* Negative contributions */
        for (int i = 0; i < 11; ++i) {
            neg_sum += cache[(size_t)(k - coeff_idx_neg[i])] *
                       (seq_t)coeff_neg[i];
        }

        /* a(k) = pos_sum - neg_sum */
        cache[(size_t)k] = pos_sum - neg_sum;
    }

    return cache[(size_t)n];
}

/*
 * A217278  Sequences A124174 and A006454 interlaced.
 * https://oeis.org/A217278
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 35*(a(n-4) - a(n-8)) + a(n-12) Initial values (n = 0 ..
 *            11): 0, 0, 1, 3, 10, 15, 45, 120, 351, 528, 1540, 4095
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 26, u64 n <= 51, u128 n <= 102,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const seq_t init_vals[12] = {
    0, 0, 1, 3, 10, 15, 45, 120, 351, 528, 1540, 4095
};

static seq_t *cache = NULL;
static size_t cached  = 0;   // number of elements currently computed
static size_t cap     = 0;   // allocated capacity

seq_t a(idx_t n) {
    if (n >= cached) {
        size_t needed = (size_t)n + 1;
        if (cap < needed) {
            seq_t *tmp = (seq_t *)realloc(cache, needed * sizeof(seq_t));
            if (!tmp) { /* allocation failed; crash or abort */
                abort();
            }
            cache = tmp;
            cap = needed;
        }
        /* Compute from cached to n inclusive */
        for (idx_t i = cached; i <= n; ++i) {
            if (i < 12) {
                cache[(size_t)i] = init_vals[(size_t)i];
            } else {
                cache[(size_t)i] = (seq_t)35 * (cache[(size_t)(i - 4)] - cache[(size_t)(i - 8)])
                                + cache[(size_t)(i - 12)];
            }
        }
        cached = (size_t)n + 1;
    }
    return cache[(size_t)n];
}

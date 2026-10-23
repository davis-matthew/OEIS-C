/*
 * A108196  Expansion of (x-1)*(x+1) / (8*x^2 + 1 - 3*x + x^4 - 3*x^3).
 * https://oeis.org/A108196
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 3*a(n‑1) - 8*a(n‑2) + 3*a(n‑3) - a(n‑4) with a(0) = -1,
 *            a(1) = -3, a(2) = 0, a(3) = 21
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 23, i64 n <= 45, i128 n <= 92,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* cached sequence values     */
static idx_t cache_len = 0;          /* number of cached elements  */
static idx_t cache_cap = 0;          /* allocated capacity         */

static const seq_t init_vals[4] = {-1, -3, 0, 21};

seq_t a(idx_t n)
{
    /* Ensure the cache is large enough for indices 0 … n */
    idx_t needed = n + 1;
    if (needed > cache_cap) {
        idx_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* unrecoverable memory failure */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i < 4) {
            val = init_vals[i];
        } else {
            val = 3 * cache[i - 1] - 8 * cache[i - 2]
                + 3 * cache[i - 3] - cache[i - 4];
        }
        cache[i] = val;
    }
    cache_len = n + 1;
    return cache[n];
}

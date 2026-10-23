/*
 * A041656  Numerators of continued fraction convergents to sqrt(347).
 * https://oeis.org/A041656
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   a(0) = 18; for n ≥ 1: a(n) = a_n · a(n – 1) + a(n – 2), where
 *            (a_n)_{n≥1} are the continued‑fraction coefficients of √347.
 *            Those coefficients form a period of length 14: [1, 1, 1, 2, 4,
 *            1, 17, 1, 4, 2, 1, 1, 1, 36]
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 20, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static const idx_t period_len = 14;
    static const seq_t period[14] = {1, 1, 1, 2, 4, 1,
                                     17, 1, 4, 2, 1, 1, 1, 36};

    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) {
            /* In the unlikely event of OOM, abort. */
            abort();
        }
        cache = new_cache;
        for (idx_t i = cache_cap; i <= n; ++i) {
            if (i == 0) {
                cache[0] = (seq_t)18;
            } else {
                idx_t idx = (i - 1) % period_len;
                seq_t a_val = period[idx];
                seq_t p_prev = cache[i - 1];
                seq_t p_prev2 = (i >= 2) ? cache[i - 2] : (seq_t)1;
                cache[i] = a_val * p_prev + p_prev2;
            }
        }
        cache_cap = new_cap;
    }

    return cache[n];
}

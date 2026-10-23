/*
 * A022657  Expansion of Product_{m>=1} (1+m*q^m)^29.
 * https://oeis.org/A022657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{m≥1} (1 + m*q^m)^29
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 25, u128 n <= 69,
 *            big any n
 * Helpers:   init_binom29
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/init_binom29.h"

/* Return the nth term of sequence A022657. */
seq_t a(idx_t n) {
    static int binom_done = 0;
    static seq_t binom[30];

    if (!binom_done) {
        init_binom29(binom);
        binom_done = 1;
    }

    if (n == 0)
        return 1;

    size_t sz = (size_t)n + 1;
    seq_t *dp_prev = (seq_t *)calloc(sz, sizeof(seq_t));
    seq_t *dp_cur  = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!dp_prev || !dp_cur) {
        /* Memory allocation failed; abort with zero. */
        free(dp_prev);
        free(dp_cur);
        return 0;
    }

    dp_prev[0] = 1;          /* initial state (no parts used) */

    for (idx_t m = 1; m <= n; ++m) {
        /* zero dp_cur for this iteration */
        for (size_t i = 0; i < sz; ++i) dp_cur[i] = 0;

        for (idx_t n0 = 0; n0 <= n; ++n0) {
            seq_t prev = dp_prev[n0];
            if (prev == 0) continue;

            idx_t max_k = (n - n0) / m;
            if (max_k > 29) max_k = 29;

            seq_t m_pow = 1;        /* m^k */
            for (idx_t k = 0; k <= max_k; ++k) {
                seq_t coef = binom[k] * m_pow;
                dp_cur[n0 + m * k] += prev * coef;
                m_pow *= (seq_t)m;
            }
        }

        /* swap dp_prev and dp_cur for next iteration */
        seq_t *tmp = dp_prev;
        dp_prev = dp_cur;
        dp_cur  = tmp;
    }

    seq_t result = dp_prev[n];

    free(dp_prev);
    free(dp_cur);

    return result;
}

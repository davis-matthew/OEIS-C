/*
 * A022657  Expansion of Product_{m>=1} (1+m*q^m)^29.
 * https://oeis.org/A022657
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of q^n in ∏_{m≥1} (1 + m*q^m)^29
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 10, u64 n <= 25, u128 n <= 69,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 963 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Compute binomial coefficients (29 choose k) once. */
static void init_binom29(seq_t binom[30]) {
    binom[0] = 1;
    for (idx_t k = 1; k <= 29; ++k) {
        binom[k] = binom[k - 1] * (seq_t)(29 - (k - 1)) / (seq_t)k;
    }
}

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
    seq_t *dp_prev = mem_new(seq_t, sz);
    seq_t *dp_cur  = mem_new(seq_t, sz);
    if (!dp_prev || !dp_cur) {
        /* Memory allocation failed; abort with zero. */
        mem_free(dp_prev);
        mem_free(dp_cur);
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

    mem_free(dp_prev);
    mem_free(dp_cur);

    return result;
}

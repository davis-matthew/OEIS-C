/*
 * A053255  Coefficients of the '3rd-order' mock theta function rho(q).
 * https://oeis.org/A053255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, changed
 * Formula:   a(n) is the coefficient of q^n in Σ_{m≥0} q^{2m(m+1)} /
 *            Π_{k=0}^m (1 + q^{2k+1} + q^{4k+2})
 * Range:     i8 n <= 176, i16 n <= 545, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t N = n;
    size_t sz = (size_t)N + 1;
    seq_t *ans  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *cur  = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *tmp  = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!ans || !cur || !tmp) {
        free(ans); free(cur); free(tmp);
        return (seq_t)0;
    }

    /* t0 = 1/(1+q+q^2)  →  pattern 1, -1, 0 repeating */
    for (size_t i = 0; i < sz; ++i) {
        int r = i % 3;
        seq_t val = (r == 0) ? 1 : ((r == 1) ? (seq_t)-1 : (seq_t)0);
        ans[i] = val;
        cur[i] = val;
    }

    for (idx_t m = 1; ; ++m) {
        /* stop when the minimal exponent 2m(m+1) exceeds N */
        unsigned __int128 shift_min128 = (unsigned __int128)2 * m * (m + 1);
        if (shift_min128 > N) break;

        idx_t a = 2 * m + 1;                     /* recurrence factor */

        /* convolution cur * 1/(1+q^{a}+q^{2a})  (periodic coefficients) */
        for (size_t i = 0; i < sz; ++i) tmp[i] = 0;
        for (size_t i = 0; i < sz; ++i) {
            seq_t c = cur[i];
            if (c == 0) continue;
            size_t max_j = (N - i) / a;          /* largest j with i+j*a ≤ N */
            for (size_t j = 0; j <= max_j; ++j) {
                size_t idx = i + j * a;
                int coeff = (j % 3 == 0) ? 1 : ((j % 3 == 1) ? -1 : 0);
                if (coeff) tmp[idx] += c * (seq_t)coeff;
            }
        }

        /* shift by 4*m to obtain t_m */
        for (size_t i = 0; i < sz; ++i) cur[i] = 0;
        size_t shift_amt = (size_t)(4 * m);
        for (size_t i = 0; i + shift_amt <= N; ++i) {
            cur[i + shift_amt] = tmp[i];
        }

        /* add contributions of t_m to ans */
        for (size_t i = 0; i < sz; ++i) {
            ans[i] += cur[i];
        }
    }

    seq_t result = ans[n];
    free(ans); free(cur); free(tmp);
    return result;
}

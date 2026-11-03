/*
 * A000525  Number of partially labeled rooted trees with n nodes (4 of which
 *          are labeled).
 * https://oeis.org/A000525
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   A(x) = B(x)^4*(64-79*B(x)+36*B(x)^2-6*B(x)^3)/(1-B(x))^7 where
 *            B(x) is g.f. for rooted trees (A000081)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 34, u128 n <= 73,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 191 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static void multiply_series(const seq_t *a, const seq_t *b, seq_t *result, idx_t n) {
    mem_set(result, 0, (n + 1) * sizeof(seq_t));
    for (idx_t i = 0; i <= n; i++) {
        for (idx_t j = 0; j <= n - i; j++) {
            result[i + j] += a[i] * b[j];
        }
    }
}

static void power_series_power(const seq_t *a, idx_t p, seq_t *result, idx_t n) {
    mem_set(result, 0, (n + 1) * sizeof(seq_t));
    result[0] = 1;
    
    seq_t *temp = mem_new(seq_t, (n + 1));
    for (idx_t k = 0; k < p; k++) {
        multiply_series(result, a, temp, n);
        mem_copy(result, temp, (n + 1) * sizeof(seq_t));
    }
    mem_free(temp);
}

seq_t a(idx_t n) {
    if (n < 4) return 0;
    
    seq_t *b = mem_new(seq_t, n + 1);
    b[1] = 1;
    
    for (idx_t i = 2; i <= n; i++) {
        seq_t sum = 0;
        for (idx_t k = 1; k < i; k++) {
            seq_t ds = 0;
            for (idx_t d = 1; d * d <= k; d++) {
                if (k % d == 0) {
                    ds += (seq_t)d * b[d];
                    if (d != k / d) {
                        ds += (seq_t)(k / d) * b[k / d];
                    }
                }
            }
            sum += ds * b[i - k];
        }
        b[i] = sum / (seq_t)(i - 1);
    }
    
    seq_t *b2 = mem_new(seq_t, (n + 1));
    seq_t *b3 = mem_new(seq_t, (n + 1));
    seq_t *b4 = mem_new(seq_t, (n + 1));
    seq_t *b5 = mem_new(seq_t, (n + 1));
    seq_t *b6 = mem_new(seq_t, (n + 1));
    seq_t *b7 = mem_new(seq_t, (n + 1));
    
    power_series_power(b, 2, b2, n);
    power_series_power(b, 3, b3, n);
    power_series_power(b, 4, b4, n);
    multiply_series(b4, b, b5, n);
    multiply_series(b5, b, b6, n);
    multiply_series(b6, b, b7, n);
    
    seq_t *num = mem_new(seq_t, (n + 1));
    for (idx_t i = 0; i <= n; i++) {
        num[i] = (seq_t)64 * b4[i] - (seq_t)79 * b5[i] + (seq_t)36 * b6[i] - (seq_t)6 * b7[i];
    }
    
    seq_t *one_minus_b = mem_new(seq_t, n + 1);
    one_minus_b[0] = 1;
    for (idx_t i = 1; i <= n; i++) {
        one_minus_b[i] = -(b[i]);
    }
    
    seq_t *denom = mem_new(seq_t, (n + 1));
    power_series_power(one_minus_b, 7, denom, n);
    
    seq_t *q = mem_new(seq_t, n + 1);
    for (idx_t i = 0; i <= n; i++) {
        q[i] = num[i];
        for (idx_t j = 1; j < i; j++) {
            q[i] -= q[j] * denom[i - j];
        }
        q[i] /= denom[0];
    }
    
    seq_t result = q[n];
    
    mem_free(b);
    mem_free(b2);
    mem_free(b3);
    mem_free(b4);
    mem_free(b5);
    mem_free(b6);
    mem_free(b7);
    mem_free(num);
    mem_free(one_minus_b);
    mem_free(denom);
    mem_free(q);
    
    return result;
}

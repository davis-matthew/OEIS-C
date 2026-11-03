/*
 * A000553  Number of labeled rooted trees of height 4 with n nodes.
 * https://oeis.org/A000553
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f.: x*(exp(x*exp(x*exp(x*exp(x)))) - exp(x*exp(x*exp(x))))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 19 (2026-11-03)
 * Helpers:   binomial, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/mem.h"

static seq_t *alloc_array(idx_t n) {
    seq_t *p = mem_new(seq_t, (n + 1));
    for (idx_t i = 0; i <= n; i++) {
        p[i] = 0;
    }
    return p;
}

static void mul_by_x(const seq_t *f, seq_t *result, idx_t N) {
    result[0] = 0;
    for (idx_t k = 1; k <= N; k++) {
        result[k] = (seq_t)k * f[k - 1];
    }
}

static void exp_egf(const seq_t *f, seq_t *g, idx_t N) {
    g[0] = 1;
    for (idx_t m = 1; m <= N; m++) {
        g[m] = 0;
        for (idx_t j = 1; j <= m; j++) {
            seq_t binom = binomial(m - 1, j - 1);
            seq_t term = binom * f[j] * g[m - j];
            g[m] += term;
        }
    }
}

seq_t a(idx_t n) {
    if (n < 5) return 0;
    
    idx_t N = n + 1;
    seq_t *exp_x = alloc_array(N);
    seq_t *x_exp_x = alloc_array(N);
    seq_t *exp_x_exp_x = alloc_array(N);
    seq_t *x_exp_x_exp_x = alloc_array(N);
    seq_t *exp_x_exp_x_exp_x = alloc_array(N);
    seq_t *x_exp_x_exp_x_exp_x = alloc_array(N);
    seq_t *exp_x_exp_x_exp_x_exp_x = alloc_array(N);
    seq_t *diff = alloc_array(N);
    seq_t *result = alloc_array(N);
    
    for (idx_t k = 0; k <= N; k++) exp_x[k] = 1;
    
    mul_by_x(exp_x, x_exp_x, N);
    exp_egf(x_exp_x, exp_x_exp_x, N);
    
    mul_by_x(exp_x_exp_x, x_exp_x_exp_x, N);
    exp_egf(x_exp_x_exp_x, exp_x_exp_x_exp_x, N);
    
    mul_by_x(exp_x_exp_x_exp_x, x_exp_x_exp_x_exp_x, N);
    exp_egf(x_exp_x_exp_x_exp_x, exp_x_exp_x_exp_x_exp_x, N);
    
    for (idx_t k = 0; k <= N; k++) {
        diff[k] = exp_x_exp_x_exp_x_exp_x[k] - exp_x_exp_x_exp_x[k];
    }
    
    mul_by_x(diff, result, N);
    
    seq_t ans = result[n];
    
    mem_free(exp_x);
    mem_free(x_exp_x);
    mem_free(exp_x_exp_x);
    mem_free(x_exp_x_exp_x);
    mem_free(exp_x_exp_x_exp_x);
    mem_free(x_exp_x_exp_x_exp_x);
    mem_free(exp_x_exp_x_exp_x_exp_x);
    mem_free(diff);
    mem_free(result);
    
    return ans;
}

/*
 * A000553  Number of labeled rooted trees of height 4 with n nodes.
 * https://oeis.org/A000553
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f.: x*(exp(x*exp(x*exp(x*exp(x)))) - exp(x*exp(x*exp(x))))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 10, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   a181_binom, alloc_array, mul_by_x
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a181_binom.h"
#include "../utils/alloc_array.h"
#include "../utils/mul_by_x.h"

static void exp_egf(const seq_t *f, seq_t *g, idx_t N) {
    g[0] = 1;
    for (idx_t m = 1; m <= N; m++) {
        g[m] = 0;
        for (idx_t j = 1; j <= m; j++) {
            seq_t binom = a181_binom(m - 1, j - 1);
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
    
    free(exp_x);
    free(x_exp_x);
    free(exp_x_exp_x);
    free(x_exp_x_exp_x);
    free(exp_x_exp_x_exp_x);
    free(x_exp_x_exp_x_exp_x);
    free(exp_x_exp_x_exp_x_exp_x);
    free(diff);
    free(result);
    
    return ans;
}

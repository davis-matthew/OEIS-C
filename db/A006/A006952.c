/*
 * A006952  Number of conjugacy classes in GL(n,3).
 * https://oeis.org/A006952
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: Product_{k>=1} (1-x^k)/(1-3*x^k); coefficients built by
 *            multiplying in each factor
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *coef = NULL;
static idx_t coef_len = 0;

static void build(idx_t len) {
    seq_t *c = alloc_zero(len);
    c[0] = 1;
    for (idx_t k = 1; k < len; k++) {
        /* multiply by 1/(1 - 3 x^k) */
        for (idx_t i = k; i < len; i++)
            c[i] += c[i - k] * 3;
        /* multiply by (1 - x^k) */
        for (idx_t i = len - 1; i >= k; i--) {
            c[i] -= c[i - k];
            if (i == k) break;
        }
    }
    if (coef != NULL) release(coef, coef_len);
    coef = c;
    coef_len = len;
}

seq_t a(idx_t n) {
    if (n >= coef_len) {
        idx_t len = coef_len * 2;
        if (len < n + 1) len = n + 1;
        if (len < 32) len = 32;
        build(len);
    }
    return coef[n];
}

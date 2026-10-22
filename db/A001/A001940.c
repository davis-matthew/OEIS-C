/*
 * A001940  Absolute value of coefficients of an elliptic function.
 * https://oeis.org/A001940
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: Product_{k>=1} (1 - x^k)^(-c(k)), c(k) = 6 if 4 does not
 *            divide k, else 0. Big build: Euler transform n*a(n) =
 *            Sum_{k=1..n} b(k)*a(n-k), a(0) = 1, with b(k) = Sum_{d|k} d*c(d)
 *            = 6*(sigma(k) - 4*sigma(k/4)) (last term only when 4|k).
 *            Fixed-width build: direct series product by repeated (1 -...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 28, u64 n <= 93, u128 n <= 323,
 *            big any n
 * Helpers:   fixed_calc, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/grow_zero.h"
#include "../utils/fixed_calc.h"

static int fixed_checked = 0;
static int fixed_flag = 0;

static int is_fixed_type(void) {
    if (!fixed_checked) {
        seq_t t = 1;
        int i;
        for (i = 0; i < 200; i++) t += t;
        fixed_flag = (t == 0);
        fixed_checked = 1;
    }
    return fixed_flag;
}

static seq_t *memo = NULL;
static idx_t memo_len = 0;
static uint64_t *bcoef = NULL;

static void build_b(idx_t len) {
    uint64_t *sig = (uint64_t *)calloc((size_t)len, sizeof(uint64_t));
    idx_t d, m, k;
    for (d = 1; d < len; d++)
        for (m = d; m < len; m += d)
            sig[m] += d;
    free(bcoef);
    bcoef = (uint64_t *)calloc((size_t)len, sizeof(uint64_t));
    for (k = 1; k < len; k++) {
        uint64_t s = sig[k];
        if (k % 4 == 0) s -= 4 * sig[k / 4];
        bcoef[k] = 6 * s;
    }
    free(sig);
}

seq_t a(idx_t n) {
    if (is_fixed_type()) return fixed_calc(n);

    if (n + 1 > memo_len) {
        idx_t newlen = n + 1;
        idx_t m, k;
        if (newlen < 2 * memo_len) newlen = 2 * memo_len;
        if (newlen < 64) newlen = 64;
        memo = grow_zero(memo, memo_len, newlen);
        build_b(newlen);
        if (memo_len == 0) {
            memo[0] = 1;
            memo_len = 1;
        }
        for (m = memo_len; m < newlen; m++) {
            seq_t s = 0;
            for (k = 1; k <= m; k++)
                s += memo[m - k] * (seq_t)bcoef[k];
            memo[m] = s / (seq_t)m;
        }
        memo_len = newlen;
    }
    return memo[n];
}

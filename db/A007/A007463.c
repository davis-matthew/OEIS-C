/*
 * A007463  Shifts left under lcm-convolution with itself.
 * https://oeis.org/A007463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(0) = 1; a(n) = Sum_{k=0..n-1} lcm(a(k), a(n-1-k)) for n >= 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 38, u128 n <= 73,
 *            big any n
 * Helpers:   gcd_seq, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/gcd_seq.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static seq_t lcm_v(seq_t x, seq_t y) {
    seq_t g = gcd_seq(x, y);
    if (g == 0) return 0;
    return x / g * y;
}

seq_t a(idx_t n) {
    if (n < memo_len) return memo[n];
    memo = grow_zero(memo, memo_len, n + 1);
    idx_t i;
    if (memo_len == 0) {
        memo[0] = 1;
        memo_len = 1;
    }
    for (i = memo_len; i <= n; i++) {
        idx_t m = i - 1;
        seq_t s = 0;
        idx_t k;
        for (k = 0; 2 * k < m; k++) {
            seq_t l = lcm_v(memo[k], memo[m - k]);
            s += l;
            s += l;
        }
        if (2 * k == m) s += memo[k];
        memo[i] = s;
        memo_len = i + 1;
    }
    return memo[n];
}

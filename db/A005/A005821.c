/*
 * A005821  a(n) = floor(tau*a(n-1)) + a(n-2) with a(0)=0 and a(1)=1.
 * https://oeis.org/A005821
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(tau*a(n-1)) + a(n-2), a(0) = 0, a(1) = 1, tau =
 *            (1+sqrt(5))/2. Exact integer method: let L(n) = floor(tau*a(n)).
 *            Then tau*a(n) = c + e with c = L(n-1) + a(n-1) + L(n-2) and -1 <
 *            e < 1, so L(n) is c or c-1. It is c exactly when c < tau*x (x =
 *            a(n)), i.e. when x^2 - x*d - d^2 > 0 with...
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 32, u64 n <= 62, u128 n <= 122,
 *            big any n
 * Helpers:   seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/seq_is_big.h"

/* Returns true if x^2 - x*d - d^2 > 0 (x > 0, d >= 0), without overflowing. */
static bool norm_positive(seq_t x, seq_t d) {
    if (seq_is_big())
        return d * (x + d) < x * x;
    bool s = true;
    for (;;) {
        if (d >= x) return !s;
        if (d == 0 || d <= x - d) return s;
        /* N(x, d) = -N(d, x - d) */
        seq_t t = x - d;
        x = d;
        d = t;
        s = !s;
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    seq_t a_prev2 = 0, a_prev1 = 1;   /* a(i-2), a(i-1) */
    seq_t L_prev2 = 0, L_prev1 = 1;   /* L(i-2), L(i-1) */
    seq_t cur = 0;
    for (idx_t i = 2; i <= n; i++) {
        cur = L_prev1 + a_prev2;
        if (i == n) break;
        seq_t c = L_prev1 + a_prev1 + L_prev2;
        seq_t d = c - cur;
        seq_t L = norm_positive(cur, d) ? c : c - 1;
        a_prev2 = a_prev1;
        a_prev1 = cur;
        L_prev2 = L_prev1;
        L_prev1 = L;
    }
    return cur;
}

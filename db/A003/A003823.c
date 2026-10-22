/*
 * A003823  Power series expansion of the Rogers-Ramanujan continued fraction
 *          1+x/(1+x^2/(1+x^3/(1+x^4/(1+...)))).
 * https://oeis.org/A003823
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   G.f. = (Sum_{k in Z} (-1)^k * x^((5*k+1)*k/2)) / (Sum_{k in Z}
 *            (-1)^k * x^((5*k+3)*k/2)), so a(n) = N(n) - Sum_{j>=1}
 *            D(j)*a(n-j), a(0) = 1, where N and D are the sparse numerator
 *            and denominator series
 * Range:     i8 n <= 62, i16 n <= 187, i32 n <= 587, i64 n <= 2002,
 *            i128 n <= 7232, big any n
 * Helpers:   grow_zero, theta_coef
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/grow_zero.h"
#include "../utils/theta_coef.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;   /* allocated length */
static idx_t memo_done = 0;  /* number of computed terms */

seq_t a(idx_t n) {
    if (n >= memo_len) {
        idx_t nl = memo_len ? memo_len : 64;
        while (nl <= n) nl *= 2;
        memo = grow_zero(memo, memo_len, nl);
        memo_len = nl;
    }
    if (memo_done == 0) {
        memo[0] = 1;
        memo_done = 1;
    }
    while (memo_done <= n) {
        long long i = (long long)memo_done;
        long long k;
        seq_t s = 0;
        int nc = theta_coef(i, 1);
        if (nc > 0) s += 1;
        else if (nc < 0) s -= 1;
        for (k = 1;; k++) {
            long long ep = k * (5 * k + 3) / 2;
            long long em = k * (5 * k - 3) / 2;
            int odd = (int)(k & 1);
            if (em > i) break;
            /* D(e) = (-1)^k; subtract D(e)*a(i-e) */
            if (odd) s += memo[i - em];
            else s -= memo[i - em];
            if (ep <= i) {
                if (odd) s += memo[i - ep];
                else s -= memo[i - ep];
            }
        }
        memo[i] = s;
        memo_done++;
    }
    return memo[n];
}

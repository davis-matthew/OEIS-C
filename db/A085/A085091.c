/*
 * A085091  Denominator of Sum_{i=2..t} (d(i)/d(i-1)-1), where d(1), ..., d(t)
 *          are the divisors of n.
 * https://oeis.org/A085091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of sum_{i=2..t} (d(i)/d(i-1)-1), where
 *            d(1)...d(t) are * the sorted divisors of n (d(1)=1, d(t)=n)
 * Range:     u8 n <= 29, u16 n <= 4249, u32 n <= 9596, u64 n <= 9596,
 *            u128 n <= 9596, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static inline idx_t gcd_idx(idx_t a, idx_t b) {
    while (b) {
        idx_t r = a % b;
        a = b;
        b = r;
    }
    return a;
}

seq_t a(idx_t n) {
    /* Gather divisors of n */
    idx_t divs[200];
    int cnt = 0;
    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            divs[cnt++] = i;
            if (i != n / i) divs[cnt++] = n / i;
        }
    }
    /* sort ascending */
    for (int i = 1; i < cnt; ++i) {
        for (int j = i; j > 0 && divs[j - 1] > divs[j]; --j) {
            idx_t tmp = divs[j];
            divs[j] = divs[j - 1];
            divs[j - 1] = tmp;
        }
    }
    if (cnt <= 1) return 1; /* n = 1 */

    /* Sum of ratios as rational P/Q */
    seq_t P = 0;
    seq_t Q = 1;
    for (int i = 1; i < cnt; ++i) {
        idx_t d_prev = divs[i - 1];
        idx_t d_cur  = divs[i];
        idx_t g      = gcd_idx(d_cur, d_prev);

        seq_t term_num = (seq_t)(d_cur / g);
        seq_t term_den = (seq_t)(d_prev / g);

        seq_t new_num = P * term_den + Q * term_num;
        seq_t new_den = Q * term_den;
        seq_t gtmp   = gcd(new_num, new_den);

        P = new_num / gtmp;
        Q = new_den / gtmp;
    }

    /* Denominator of R - (t-1) */
    seq_t t_minus1 = (seq_t)(cnt - 1);
    seq_t numerS   = P - t_minus1 * Q;
    seq_t g2       = gcd(numerS, Q);
    return Q / g2;
}

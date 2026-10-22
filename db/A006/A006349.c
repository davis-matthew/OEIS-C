/*
 * A006349  Related to series-parallel networks.
 * https://oeis.org/A006349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n} d*A000084(d) (= A058352(n) + n*A000084(n)),
 *            where A000084 comes from the Euler transform: if T(x) = x +
 *            S(x), with S the series-type networks, then Prod_{k>=1}
 *            (1-x^k)^(-t_k) = 1 + 2T(x) - x, A000084(1) = 1, and A000084(n) =
 *            2*t_n for n >= 2
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_zero, grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"

static seq_t *T = 0;   /* t_n */
static seq_t *B = 0;   /* b_n = coefficient of 1 + 2T - x; b_n = A000084(n) for n >= 1 */
static seq_t *C = 0;   /* c_n = Sum_{d|n} d*t_d */
static idx_t have = 0; /* indices 0..have-1 computed */

static seq_t proper_div_sum(idx_t m) {
    seq_t s = 0;
    idx_t d;
    for (d = 1; d * d <= m; d++) {
        if (m % d == 0) {
            idx_t e = m / d;
            if (d < m) s += (seq_t)d * T[d];
            if (e != d && e < m) s += (seq_t)e * T[e];
        }
    }
    return s;
}

static void ensure(idx_t n) {
    idx_t newlen, m, k;
    if (n < have) return;
    newlen = n + 1;
    if (newlen < 2) newlen = 2;
    if (!T) {
        T = alloc_zero(newlen);
        B = alloc_zero(newlen);
        C = alloc_zero(newlen);
    } else {
        T = grow_zero(T, have, newlen);
        B = grow_zero(B, have, newlen);
        C = grow_zero(C, have, newlen);
    }
    for (m = have; m < newlen; m++) {
        if (m == 0) {
            T[0] = 0; B[0] = 1; C[0] = 0;
        } else if (m == 1) {
            T[1] = 1; B[1] = 1; C[1] = 1;
        } else {
            seq_t s = proper_div_sum(m);
            seq_t acc = s;
            for (k = 1; k < m; k++) acc += C[k] * B[m - k];
            T[m] = acc / (seq_t)m;
            B[m] = T[m] * 2;
            C[m] = s + (seq_t)m * T[m];
        }
    }
    have = newlen;
}

seq_t a(idx_t n) {
    seq_t r = 0;
    idx_t d;
    if (n < 1) return 0;
    ensure(n);
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            idx_t e = n / d;
            r += (seq_t)d * B[d];
            if (e != d) r += (seq_t)e * B[e];
        }
    }
    return r;
}

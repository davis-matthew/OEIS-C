/*
 * A004407  Expansion of ( Sum_{n = -infinity..infinity} x^(n^2) )^(-6).
 * https://oeis.org/A004407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. 1/theta_3(x)^6, theta_3(x) = 1 + 2*Sum_{k>=1} x^(k^2);
 *            computed by dividing 1 by theta_3 six times: f_j(n) = f_{j-1}(n)
 *            - 2*Sum_{k>=1, k^2<=n} f_j(n-k^2), f_0 = 1
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 17, i64 n <= 54, i128 n <= 182,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *A004407_tab = 0;
static idx_t A004407_len = 0;

static void A004407_build(idx_t L) {
    seq_t *p = alloc_zero(L);
    seq_t *q = alloc_zero(L);
    seq_t *t;
    int pass;
    idx_t i, k;
    p[0] = 1;
    for (pass = 0; pass < 6; pass++) {
        for (i = 0; i < L; i++) {
            seq_t s = p[i];
            for (k = 1; k * k <= i; k++) {
                s -= q[i - k * k];
                s -= q[i - k * k];
            }
            q[i] = s;
        }
        t = p; p = q; q = t;
    }
    release(q, L);
    if (A004407_tab) release(A004407_tab, A004407_len);
    A004407_tab = p;
    A004407_len = L;
}

seq_t a(idx_t n) {
    if (n >= A004407_len) {
        idx_t L = n + 1;
        if (L < 2 * A004407_len) L = 2 * A004407_len;
        if (L < 64) L = 64;
        A004407_build(L);
    }
    return A004407_tab[n];
}

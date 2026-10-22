/*
 * A002992  Number of n-node trees with a forbidden limb of length 6.
 * https://oeis.org/A002992
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; for n >= 1, a(n) = [n <= 6] + [x^(n-1)] (MSET_{>=3}(T)
 *            + MSET_2(K)) - [x^n] (K(x)^2 - K(x^2))/2, where T(x) = K(x) + x
 *            + x^2 + x^3 + x^4 + x^5 counts planted subtrees hanging from a
 *            branching node, and K(x) counts planted subtrees that contain a
 *            branching node: K(x) = x*(MSET(T) - 1 - T...
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 28, u64 n <= 50, u128 n <= 93,
 *            big any n
 * Helpers:   A2992_c2, A2992_c2plus, A2992_mc, A2992_mset2, A2992_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/A2992_c2.h"
#include "../utils/A2992_c2plus.h"
#include "../utils/A2992_mc.h"
#include "../utils/A2992_mset2.h"
#include "../utils/A2992_width.h"

#define A2992_CAP 16384

static seq_t A2992_E[A2992_CAP], A2992_T[A2992_CAP], A2992_K[A2992_CAP], A2992_C[A2992_CAP];
static idx_t A2992_done = 0;
static int A2992_w = -1;

static void A2992_fixed_fill(idx_t N) {
    idx_t m, k, q;
    seq_t mcs[300];
    for (k = 0; k <= N; k++) { A2992_E[k] = 0; A2992_T[k] = 0; A2992_K[k] = 0; }
    A2992_E[0] = 1;
    for (m = 1; m <= N; m++) {
        if (m >= 2) A2992_K[m] = A2992_E[m - 1] - A2992_T[m - 1] + A2992_K[m - 1];
        else A2992_K[m] = 0;
        A2992_T[m] = A2992_K[m];
        if (m <= 5) A2992_T[m] += 1;
        for (q = 1; q * m <= N; q++) mcs[q] = A2992_mc(A2992_T[m], q);
        for (k = N; k >= m; k--) {
            seq_t s = 0;
            for (q = 1; q * m <= k; q++) s += mcs[q] * A2992_E[k - q * m];
            A2992_E[k] += s;
        }
    }
    A2992_done = N + 1;
}

static void A2992_big_fill(idx_t n) {
    idx_t m, d, k;
    if (A2992_done == 0) {
        A2992_E[0] = 1; A2992_T[0] = 0; A2992_K[0] = 0; A2992_C[0] = 0;
        A2992_done = 1;
    }
    for (m = A2992_done; m <= n; m++) {
        seq_t s = 0;
        if (m >= 2) A2992_K[m] = A2992_E[m - 1] - A2992_T[m - 1] + A2992_K[m - 1];
        else A2992_K[m] = 0;
        A2992_T[m] = A2992_K[m];
        if (m <= 5) A2992_T[m] += 1;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                s += (seq_t)d * A2992_T[d];
                if (e != d) s += (seq_t)e * A2992_T[e];
            }
        }
        A2992_C[m] = s;
        s = 0;
        for (k = 1; k <= m; k++) s += A2992_C[k] * A2992_E[m - k];
        A2992_E[m] = s / m;
    }
    if (n + 1 > A2992_done) A2992_done = n + 1;
}

seq_t a(idx_t n) {
    idx_t m, i;
    seq_t v, kp, res;
    if (n == 0) return 1;
    if (A2992_w < 0) {
        A2992_w = A2992_width();
        if (A2992_w > 0) A2992_fixed_fill((idx_t)(2 * A2992_w + 10));
    }
    if (A2992_w > 0) {
        if (n >= A2992_done) return 0; /* cannot fit in seq_t */
    } else {
        if (n >= A2992_CAP) return 0;
        if (n >= A2992_done) A2992_big_fill(n);
    }
    m = n - 1;
    v = 0;
    if (m >= 1) v = A2992_E[m] - A2992_T[m] - A2992_mset2(A2992_T, m);
    v += A2992_mset2(A2992_K, m);
    kp = 0;
    for (i = 1; 2 * i < n; i++) kp += A2992_K[i] * A2992_K[n - i];
    if (n % 2 == 0) kp += A2992_c2(A2992_K[n / 2]);
    res = v - kp;
    if (n <= 6) res += 1;
    return res;
}

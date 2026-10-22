/*
 * A000949  Number of forests with n nodes and height at most 2.
 * https://oeis.org/A000949
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   E.g.f. A(x) = exp(x*exp(x*exp(x))). With C(x) = exp(x*exp(x))
 *            (A000248): c(0) = 1, c(m) = Sum_{k=1..m}
 *            binomial(m-1,k-1)*k*c(m-k); a(0) = 1, a(m) = Sum_{k=1..m}
 *            binomial(m-1,k-1)*k*c(k-1)*a(m-k)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 18, u128 n <= 31,
 *            big any n
 * Helpers:   a306_alloc0, a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a306_alloc0.h"
#include "../utils/a306_grow.h"

static seq_t *a949_A = 0;
static seq_t *a949_C = 0;
static idx_t a949_len = 0; /* number of computed entries */
static idx_t a949_cap = 0;

static void a949_extend(idx_t n)
{
    idx_t m, k;
    if (n < a949_len) return;
    if (n + 1 > a949_cap) {
        idx_t nc = a949_cap ? a949_cap : 16;
        while (nc < n + 1) nc *= 2;
        if (a949_A == 0) {
            a949_A = a306_alloc0(nc);
            a949_C = a306_alloc0(nc);
        } else {
            a949_A = a306_grow(a949_A, a949_cap, nc);
            a949_C = a306_grow(a949_C, a949_cap, nc);
        }
        a949_cap = nc;
    }
    if (a949_len == 0) {
        a949_A[0] = 1;
        a949_C[0] = 1;
        a949_len = 1;
    }
    for (m = a949_len; m <= n; m++) {
        seq_t sc = 0, sa = 0;
        seq_t bin = 1; /* binomial(m-1, k-1) */
        for (k = 1; k <= m; k++) {
            seq_t kk = (seq_t)k;
            sc += bin * kk * a949_C[m - k];
            sa += bin * kk * a949_C[k - 1] * a949_A[m - k];
            if (k < m) bin = bin * (seq_t)(m - k) / kk;
        }
        a949_C[m] = sc;
        a949_A[m] = sa;
    }
    a949_len = n + 1;
}

seq_t a(idx_t n)
{
    a949_extend(n);
    return a949_A[n];
}

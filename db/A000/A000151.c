/*
 * A000151  Number of oriented rooted trees with n nodes. Also rooted trees
 *          with n nodes and 2-colored non-root nodes.
 * https://oeis.org/A000151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen, nice
 * Formula:   A(x) = x*Product_{k>=1} 1/(1-x^k)^(2*a(k)), a(1) = 1.
 *            Fixed-width builds multiply the forest series by
 *            1/(1-x^k)^(2a(k)) using exactly reduced binomials C(2a(k)+j-1,
 *            j), so every intermediate is <= a(n). Arbitrary-precision builds
 *            use the equivalent recurrence a(n+1) = (1/n)*Sum_{k=1..n}
 *            c(k)*a(n...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 55,
 *            big any n
 * Helpers:   a151_gcd, a151_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/a151_gcd.h"
#include "../utils/a151_width.h"

#define A151_MAXN 10010
#define A151_FIX 300

static seq_t a151_fixed(idx_t n)
{
    seq_t F[A151_FIX];
    seq_t C[A151_FIX];
    idx_t k, t, j, jmax, i;
    for (i = 0; i < n; i++) F[i] = 0;
    F[0] = 1;
    for (k = 1; k + 1 <= n; k++) {
        seq_t m = F[k - 1] * 2;        /* 2*a(k) */
        jmax = (n - 1) / k;
        C[0] = 1;
        for (j = 1; j <= jmax; j++) {
            seq_t d = (seq_t)j;
            seq_t c = C[j - 1];
            seq_t g = a151_gcd(c, d);
            c /= g;
            c *= (m + (seq_t)(j - 1)) / (d / g);
            C[j] = c;
        }
        for (t = n - 1; t >= k; t--) {
            seq_t s = 0;
            for (j = 1; j * k <= t; j++)
                s += C[j] * F[t - j * k];
            F[t] += s;
        }
    }
    return F[n - 1];
}

static seq_t a151_A[A151_MAXN + 2];
static seq_t a151_C[A151_MAXN + 2];
static idx_t a151_have = 0;

static seq_t a151_rec(idx_t n)
{
    if (n > A151_MAXN) return 0;
    if (a151_have == 0) {
        a151_A[1] = 1;
        a151_C[1] = 2;
        a151_have = 1;
    }
    while (a151_have < n) {
        idx_t m = a151_have, k, d, q;
        seq_t S = 0, c = 0;
        for (k = 1; k <= m; k++)
            S += a151_C[k] * a151_A[m + 1 - k];
        a151_A[m + 1] = S / (seq_t)m;
        q = m + 1;
        for (d = 1; d * d <= q; d++) {
            if (q % d == 0) {
                idx_t e = q / d;
                c += (seq_t)d * 2 * a151_A[d];
                if (e != d) c += (seq_t)e * 2 * a151_A[e];
            }
        }
        a151_C[m + 1] = c;
        a151_have = m + 1;
    }
    return a151_A[n];
}

seq_t a(idx_t n)
{
    static int w = -1;
    if (n < 1) return 0;
    if (w < 0) w = a151_width();
    if (w > 0 && n <= (idx_t)(2 * w) && n < A151_FIX)
        return a151_fixed(n);
    return a151_rec(n);
}

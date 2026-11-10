/*
 * A063881  Number of oriented trees rooted at an arc.
 * https://oeis.org/A063881
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{i=1..n-1} r(i)*r(n-i), where r = A000151 (g.f. A(x)
 *            = B(x)^2, B(x) = g.f. of A000151); r(1) = 1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 55,
 *            big any n
 * Validated: standard, pass, 40 terms through n = 96 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd.h"

static int A000151_width(void)
{
    seq_t x = 1;
    int w = 0;
    while (x != 0 && w < 400) {
        x *= 2;
        w++;
    }
    return (x == 0) ? w : 0;
}

#define A151_MAXN 10010
#define A151_FIX 300

static seq_t A000151_fixed(idx_t n)
{
    seq_t F[A151_FIX];
    seq_t C[A151_FIX];
    idx_t k, t, j, jmax, i;
    for (i = 0; i < n; i++) F[i] = 0;
    F[0] = 1;
    for (k = 1; k + 1 <= n; k++) {
        seq_t m = F[k - 1] * 2;
        jmax = (n - 1) / k;
        C[0] = 1;
        for (j = 1; j <= jmax; j++) {
            seq_t d = (seq_t)j;
            seq_t c = C[j - 1];
            seq_t g = gcd(c, d);
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

static seq_t A000151_A[A151_MAXN + 2];
static seq_t A000151_C[A151_MAXN + 2];
static idx_t A000151_have = 0;

static seq_t A000151_rec(idx_t n)
{
    if (n > A151_MAXN) return 0;
    if (A000151_have == 0) {
        A000151_A[1] = 1;
        A000151_C[1] = 2;
        A000151_have = 1;
    }
    while (A000151_have < n) {
        idx_t m = A000151_have, k, d, q;
        seq_t S = 0, c = 0;
        for (k = 1; k <= m; k++)
            S += A000151_C[k] * A000151_A[m + 1 - k];
        A000151_A[m + 1] = S / (seq_t)m;
        q = m + 1;
        for (d = 1; d * d <= q; d++) {
            if (q % d == 0) {
                idx_t e = q / d;
                c += (seq_t)d * 2 * A000151_A[d];
                if (e != d) c += (seq_t)e * 2 * A000151_A[e];
            }
        }
        A000151_C[m + 1] = c;
        A000151_have = m + 1;
    }
    return A000151_A[n];
}

static seq_t A000151_a(idx_t n)
{
    static int w = -1;
    if (n < 1) return 0;
    if (w < 0) w = A000151_width();
    if (w > 0 && n <= (idx_t)(2 * w) && n < A151_FIX)
        return A000151_fixed(n);
    return A000151_rec(n);
}

seq_t a(idx_t n)
{
    idx_t i;
    seq_t s = 0;
    if (n < 2) return 0;
    for (i = 1; i < n; i++)
        s += A000151_a(i) * A000151_a(n - i);
    return s;
}

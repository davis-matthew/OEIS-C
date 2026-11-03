/*
 * A006376  Number of indecomposable positive definite ternary quadratic forms
 *          of determinant n.
 * https://oeis.org/A006376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   search. a(n) = T(n) - D(n), where T(n) = number of
 *            Eisenstein-reduced classically integral ternary forms a x^2+b
 *            y^2+c z^2+r yz+s xz+t xy (r,s,t even) of determinant n (one per
 *            isometry class), D(n) = #{x<=y<=z : x*y*z = n} + Sum_{k|n}
 *            B(n/k) (decomposable classes), B(m) = number of reduced binar...
 * Range:     u8 n <= 250, u16 n <= 250, u32 n <= 250, u64 n <= 250,
 *            u128 n <= 250, big any n
 * Validated: standard, pass, 110 terms through n = 246 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static long long binary_indec(long long m)
{
    long long cnt = 0, f, p;
    for (f = 1; 3 * f * f <= m; f++) {
        long long prod = m + f * f;
        for (p = 2 * f; p * p <= prod; p++) {
            if (prod % p == 0 && prod / p >= p) cnt++;
        }
    }
    return cnt;
}

static long long ternary_total(long long n)
{
    long long cnt = 0, a, b, r, s, t;
    for (a = 1; a * a * a <= 2 * n; a++) {
        for (b = a; a * b * b <= 2 * n; b++) {
            for (t = -a; t <= a; t++) {
                long long den;
                if (t & 1) continue;
                den = 4 * a * b - t * t;
                for (s = -a; s <= a; s++) {
                    if (s & 1) continue;
                    for (r = -b; r <= b; r++) {
                        long long num, c, sum;
                        if (r & 1) continue;
                        if (!((r > 0 && s > 0 && t > 0) || (r <= 0 && s <= 0 && t <= 0)))
                            continue;
                        num = 4 * n - r * s * t + a * r * r + b * s * s;
                        if (num <= 0 || num % den != 0) continue;
                        c = num / den;
                        if (c < b) continue;
                        sum = a + b + r + s + t;
                        if (sum < 0) continue;
                        if (a == t && s > 2 * r) continue;
                        if (a == s && t > 2 * r) continue;
                        if (b == r && t > 2 * s) continue;
                        if (a == -t && s != 0) continue;
                        if (a == -s && t != 0) continue;
                        if (b == -r && t != 0) continue;
                        if (sum == 0 && 2 * a + 2 * s + t > 0) continue;
                        if (a == b && llabs(r) > llabs(s)) continue;
                        if (b == c && llabs(s) > llabs(t)) continue;
                        cnt++;
                    }
                }
            }
        }
    }
    return cnt;
}

seq_t a(idx_t n)
{
    long long N = (long long)n, x, y, k;
    long long total = ternary_total(N);
    long long dec = 0;

    for (x = 1; x * x * x <= N; x++) {
        long long rem;
        if (N % x) continue;
        rem = N / x;
        for (y = x; y * y <= rem; y++) {
            if (rem % y == 0) dec++;
        }
    }
    for (k = 1; k <= N; k++) {
        if (N % k == 0) dec += binary_indec(N / k);
    }
    return (seq_t)(total - dec);
}

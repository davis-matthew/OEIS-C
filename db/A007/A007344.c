/*
 * A007344  Number of homogeneous primitive partition identities of degree 6
 *          with largest part n.
 * https://oeis.org/A007344
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_s C(c3(s),2) - n*Sum_s C(c2(s),2), where ck(s) =
 *            number of k-multisets of {1..n} with sum s (coefficients of the
 *            Gaussian binomial [n+k-1,k]_q). This counts unordered pairs of
 *            disjoint equal-sum 3-multisets of {1..n}, i.e. homogeneous
 *            primitive partition identities a1+a2+a3 = b1+b2+b3...
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 120, u64 n <= 120,
 *            u128 n <= 120, big any n
 * Validated: standard, pass, 53 terms through n = 120 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* coefficients of [n+k-1 choose k]_q into p; returns degree */
static long long gauss_coeffs(long long *p, long long len, idx_t n, int k)
{
    long long deg = 0, j;
    int i;
    for (j = 0; j < len; j++) p[j] = 0;
    p[0] = 1;
    for (i = 1; i <= k; i++) {
        long long m = (long long)n - 1 + i;
        for (j = deg + m; j >= m; j--) p[j] -= p[j - m];
        deg += m;
        for (j = i; j <= deg; j++) p[j] += p[j - i];
        deg -= i;
    }
    return deg;
}

static seq_t sum_pairs(const long long *p, long long deg)
{
    seq_t s = 0;
    long long j;
    for (j = 0; j <= deg; j++) {
        uint64_t c = (uint64_t)p[j];
        if (c >= 2) {
            uint64_t t = (c % 2 == 0) ? (c / 2) * (c - 1) : c * ((c - 1) / 2);
            s += (seq_t)t;
        }
    }
    return s;
}

seq_t a(idx_t n)
{
    long long len = 3 * (long long)n + 8;
    long long *p = (long long *)malloc((size_t)len * sizeof(long long));
    long long d;
    seq_t s2, s3;
    d = gauss_coeffs(p, len, n, 2);
    s2 = sum_pairs(p, d);
    d = gauss_coeffs(p, len, n, 3);
    s3 = sum_pairs(p, d);
    free(p);
    return s3 - (seq_t)n * s2;
}

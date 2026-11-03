/*
 * A001293  Leech triangle: k-th number (0 <= k <= n) in n-th row (0 <= n) is
 *          number of octads in S(5,8,24) containing k given points and
 *          missing n-k given points.
 * https://oeis.org/A001293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, fini, full, nice
 * Formula:   T(r,k) = Sum_{j=0..r-k} (-1)^j*binomial(r-k,j)*L(k+j), where
 *            L(i) = binomial(24-i,5-i)/binomial(8-i,5-i) for i <= 5 and L(i)
 *            = 1 for 6 <= i <= 8 (the given points lie in a common octad);
 *            finite triangle rows 0..8, read by rows
 * Range:     u8 none, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Validated: full, pass, 45 terms through n = 44 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static long long binom_ll(int m, int k) {
    long long r = 1;
    int t;
    if (k < 0 || k > m) return 0;
    for (t = 0; t < k; t++) r = r * (m - t) / (t + 1);
    return r;
}

static long long lam_val(int i) {
    if (i >= 5) return 1;
    long long num = 1, den = 1;
    int t;
    for (t = 0; t < 5 - i; t++) {
        num = num * (24 - i - t) / (t + 1);
        den = den * (8 - i - t) / (t + 1);
    }
    return num / den;
}

seq_t a(idx_t n) {
    if (n >= 45) return (seq_t)0;
    int row = 0;
    idx_t total = 0;
    while (total + (idx_t)row + 1 <= n) {
        total += (idx_t)row + 1;
        row++;
    }
    int k = (int)(n - total);
    int m = row - k;
    long long s = 0;
    int j;
    for (j = 0; j <= m; j++) {
        long long term = binom_ll(m, j) * lam_val(k + j);
        if (j & 1) s -= term; else s += term;
    }
    return (seq_t)(unsigned long long)s;
}

/*
 * A000391  Euler transform of A000332.
 * https://oeis.org/A000391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Euler transform of b(k) = A000332(k+3) = C(k+3,4): Sum_{n>=0}
 *            a(n) x^n = Prod_{k>=1} 1/(1-x^k)^C(k+3,4), a(0) = 1. Arbitrary
 *            precision: c(k) = Sum_{d|k} d*C(d+3,4), a(n) =
 *            (1/n)*Sum_{k=1..n} c(k)*a(n-k). Fixed width: multiply out the
 *            product, using 1/(1-x^k)^b = Sum_j C(b+j-1,j) x^(kj)
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 21, u64 n <= 48, u128 n <= 110,
 *            big any n
 * Helpers:   bseq, lcl_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/bseq.h"
#include "../utils/lcl_gcd.h"

#define MAXN 20001

static seq_t A[MAXN];
static seq_t Cc[MAXN];
static seq_t B[MAXN];
static idx_t have = 0;
static int mode = -1;   /* 0 = fixed width, 1 = arbitrary precision */
static idx_t cap = 0;

static void init_fixed(void) {
    idx_t i, k, j, m;
    for (i = 0; i <= cap; i++) A[i] = 0;
    A[0] = 1;
    for (k = 1; k <= cap; k++) {
        idx_t J = cap / k;
        uint64_t bu = (uint64_t)k * (k + 1) * (k + 2) * (k + 3) / 24;
        seq_t bk = (seq_t)bu;
        B[0] = 1;
        for (j = 1; j <= J; j++) {
            seq_t X = B[j - 1];
            unsigned long long r = (unsigned long long)(long long)(X % (seq_t)j);
            unsigned long long g = lcl_gcd(r, (unsigned long long)j);
            seq_t q = X / (seq_t)g;
            seq_t y = (bk + (seq_t)(j - 1)) / (seq_t)(j / g);
            B[j] = q * y;
        }
        for (m = cap; m >= k; m--) {
            seq_t s = 0;
            for (j = 1; j * k <= m; j++) s += B[j] * A[m - j * k];
            A[m] += s;
        }
    }
}

seq_t a(idx_t n) {
    if (mode < 0) {
        seq_t t = 1;
        int w = 0;
        while (w < 300 && t != 0) { t = t + t; w++; }
        if (t == 0) {
            mode = 0;
            cap = (w <= 8) ? 200 : 600;
            init_fixed();
        } else {
            mode = 1;
            A[0] = 1;
            have = 0;
        }
    }
    if (n == 0) return 1;
    if (mode == 0) {
        if (n > cap) return 0;
        return A[n];
    }
    if (n >= MAXN) return 0;
    while (have < n) {
        idx_t m = have + 1, d, k;
        seq_t c = 0, s = 0;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                c += (seq_t)d * bseq(d);
                if (d != m / d) c += (seq_t)(m / d) * bseq(m / d);
            }
        }
        Cc[m] = c;
        for (k = 1; k <= m; k++) s += Cc[k] * A[m - k];
        A[m] = s / (seq_t)m;
        have = m;
    }
    return A[n];
}

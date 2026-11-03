/*
 * A002614  Glaisher's function theta(n) (18 squares version).
 * https://oeis.org/A002614
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (A321546(n) - A002288(n))/17, where A321546(n) =
 *            Sum_{d|n} (-1)^(d+1)*d^7 and A002288(n) = [q^n] q*Prod_{k>=1}
 *            (1-q^k)^8*(1-q^(2k))^8 (eta(q)^8*eta(q^2)^8). The eta product is
 *            expanded with sparse Jacobi (eta^3) and Euler (eta) series
 * Range:     i8 n <= 2, i16 n <= 6, i32 n <= 32, i64 n <= 500, i128 n <= 500,
 *            big any n
 * Validated: standard, pass, 43 terms through n = 484 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* add (-1)^(d+1) * d^7, kept as 17*Q + R to avoid overflow */
static void a2614_add(idx_t d, seq_t *qs, long long *rs) {
    seq_t Q = 0;
    long long R = 1;
    seq_t sd = (seq_t)d;
    int i;
    for (i = 0; i < 7; i++) {
        long long t = R * (long long)d;
        Q = Q * sd + (seq_t)(t / 17);
        R = t % 17;
    }
    if (d & 1) { *qs += Q; *rs += R; }
    else { *qs -= Q; *rs -= R; }
}

/* multiply series b (length L) by Prod (1-q^(s k)) = Sum (-1)^k q^(s k(3k-1)/2) */
static void a2614_e1(long long *b, idx_t L, idx_t s) {
    idx_t m = L;
    while (m-- > 0) {
        long long v = b[m];
        idx_t k;
        for (k = 1; ; k++) {
            idx_t e1 = s * (k * (3 * k - 1) / 2);
            idx_t e2 = s * (k * (3 * k + 1) / 2);
            if (e1 > m) break;
            if (k & 1) v -= b[m - e1]; else v += b[m - e1];
            if (e2 <= m) {
                if (k & 1) v -= b[m - e2]; else v += b[m - e2];
            }
        }
        b[m] = v;
    }
}

/* multiply series b (length L) by Prod (1-q^(s k))^3 = Sum (-1)^j (2j+1) q^(s j(j+1)/2) */
static void a2614_e3(long long *b, idx_t L, idx_t s) {
    idx_t m = L;
    while (m-- > 0) {
        long long v = b[m];
        idx_t j;
        for (j = 1; ; j++) {
            idx_t e = s * (j * (j + 1) / 2);
            if (e > m) break;
            if (j & 1) v -= (long long)(2 * j + 1) * b[m - e];
            else v += (long long)(2 * j + 1) * b[m - e];
        }
        b[m] = v;
    }
}

static long long *a2614_cbuf = NULL;
static idx_t a2614_len = 0;

static long long a2614_tau(idx_t n) {
    if (n > a2614_len) {
        idx_t L = n;
        long long *b;
        if (L < 2 * a2614_len) L = 2 * a2614_len;
        if (L < 64) L = 64;
        b = (long long *)calloc((size_t)L, sizeof(long long));
        b[0] = 1;
        a2614_e3(b, L, 1);
        a2614_e3(b, L, 1);
        a2614_e1(b, L, 1);
        a2614_e1(b, L, 1);
        a2614_e3(b, L, 2);
        a2614_e3(b, L, 2);
        a2614_e1(b, L, 2);
        a2614_e1(b, L, 2);
        free(a2614_cbuf);
        a2614_cbuf = b;
        a2614_len = L;
    }
    return a2614_cbuf[n - 1];
}

seq_t a(idx_t n) {
    long long c = a2614_tau(n);
    seq_t qs = 0;
    long long rs = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            a2614_add(d, &qs, &rs);
            if (d * d != n) a2614_add(n / d, &qs, &rs);
        }
    }
    return qs + (seq_t)((rs - c) / 17);
}

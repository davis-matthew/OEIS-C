/*
 * A000669  Number of series-reduced planted trees with n leaves. Also the
 *          number of essentially series series-parallel networks with n
 *          edges; also the number of essentially parallel series-parallel
 *          networks with n edges.
 * https://oeis.org/A000669
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   Product_{k>0} 1/(1-x^k)^a_k = 1+x+2*Sum_{k>1} a_k*x^k, a(1) = 1.
 *            Taking the coefficient of x^n gives a(n) = [x^n] Product_{k<n}
 *            1/(1-x^k)^a(k). Equivalently, using the Euler transform with
 *            c(k) = Sum_{d|k} d*a(d): a(n) = ( Sum_{k=1..n-1} c(k)*b(n-k) +
 *            Sum_{d|n,d<n} d*a(d) ) / n, where b(0) = 1, b...
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 22, u64 n <= 40, u128 n <= 76,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 932 (2026-11-03)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/mem.h"

/* Fixed-width build: multiply in the factors 1/(1-x^k)^a(k) one at a time,
   with no division of large values, so every intermediate stays <= a(n). */
static seq_t fixed_compute(idx_t n) {
    seq_t *P = mem_new(seq_t, (size_t)(n + 1));
    seq_t *e = mem_new(seq_t, (size_t)(n + 1));
    idx_t m, k, j;
    seq_t res;
    for (m = 0; m <= n; m++) P[m] = 0;
    P[0] = 1;
    for (k = 1; k < n; k++) {
        seq_t ak = (k == 1) ? (seq_t)1 : P[k];
        idx_t J = n / k;
        e[0] = 1;
        for (j = 1; j <= J; j++) {
            /* e[j] = e[j-1] * (ak + j - 1) / j, computed without overflow */
            seq_t c = e[j - 1];
            seq_t t = ak + (seq_t)(j - 1);
            unsigned long long jj = (unsigned long long)j;
            unsigned long long g = gcd_u64((unsigned long long)(c % (seq_t)jj), jj);
            c = c / (seq_t)g;
            jj /= g;
            e[j] = c * (t / (seq_t)jj);
        }
        for (m = n; m >= k; m--) {
            seq_t s = P[m];
            for (j = 1; j * k <= m; j++) s += e[j] * P[m - j * k];
            P[m] = s;
            if (m == k) break;
        }
    }
    res = (n == 1) ? (seq_t)1 : P[n];
    mem_free(P);
    mem_free(e);
    return res;
}

static seq_t *grow_local(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static int seq_is_wide(void) {
    seq_t x = 1;
    int i;
    for (i = 0; i < 160; i++) x = x * 2;
    return x != 0;
}

static seq_t *A = NULL, *B = NULL, *C = NULL;
static idx_t have = 0, cap = 0;

static seq_t big_compute(idx_t n) {
    idx_t m, k, d;
    if (have == 0) {
        cap = 64;
        A = grow_local(NULL, 0, cap);
        B = grow_local(NULL, 0, cap);
        C = grow_local(NULL, 0, cap);
        A[1] = 1; B[0] = 1; B[1] = 1; C[1] = 1;
        have = 1;
    }
    if (n < cap && n <= have) return A[n];
    if (n >= cap) {
        idx_t nc = cap;
        while (nc <= n) nc *= 2;
        A = grow_local(A, cap, nc);
        B = grow_local(B, cap, nc);
        C = grow_local(C, cap, nc);
        cap = nc;
    }
    for (m = have + 1; m <= n; m++) {
        seq_t S = 0, D = 0;
        for (k = 1; k < m; k++) S += C[k] * B[m - k];
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t q = m / d;
                if (d < m) D += (seq_t)d * A[d];
                if (q != d && q < m) D += (seq_t)q * A[q];
            }
        }
        A[m] = (S + D) / (seq_t)m;
        B[m] = A[m] * 2;
        C[m] = D + (seq_t)m * A[m];
        have = m;
    }
    return A[n];
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (seq_is_wide()) return big_compute(n);
    /* beyond n = 200 the value overflows any fixed width of at most 128 bits */
    if (n > 200) return 0;
    return fixed_compute(n);
}

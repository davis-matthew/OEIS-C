/*
 * A000264  Number of 3-edge-connected rooted cubic maps with 2n nodes and a
 *          distinguished Hamiltonian cycle.
 * https://oeis.org/A000264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   b(n) = (2n)!*(2n+2)!/(2*n!*(n+1)!^2*(n+2)!) =
 *            Catalan(n)*Catalan(n+1)/2, B(x) = Sum_{n>=1} b(n) x^n, C(x) =
 *            x*(1+2*B(x))^2, and B(x) = A(C(x)). Solved term by term: a(k) =
 *            [x^k] (B(x) - Sum_{j<k} a(j)*C(x)^j)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 23, u128 n <= 42,
 *            big any n
 * Helpers:   inv_odd_mod, is_fixed, zalloc_s
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/inv_odd_mod.h"
#include "../utils/is_fixed.h"
#include "../utils/zalloc_s.h"

static seq_t *A_memo = 0;
static idx_t A_N = 0;

static void build(idx_t N)
{
    seq_t *b = zalloc_s(N + 1), *d = zalloc_s(N + 1), *c = zalloc_s(N + 1);
    seq_t *S = zalloc_s(N + 1), *P = zalloc_s(N + 1), *T = zalloc_s(N + 1);
    seq_t *a = zalloc_s(N + 1);
    idx_t i, j, k;
    int fixed = is_fixed();

    /* b(n) */
    if (fixed) {
        seq_t o = 1;
        long long e = 0;
        b[1] = 1;
        for (k = 2; k <= N; k++) {
            uint64_t d1 = k + 1, d2 = k + 2;
            o = o * (seq_t)(2 * k - 1);
            o = o * (seq_t)(2 * k + 1);
            e += 2;
            while ((d1 & 1) == 0) { d1 >>= 1; e--; }
            while ((d2 & 1) == 0) { d2 >>= 1; e--; }
            o = o * inv_odd_mod((seq_t)d1);
            o = o * inv_odd_mod((seq_t)d2);
            {
                seq_t r = o;
                long long t;
                for (t = 0; t < e && t < 200; t++) r = r + r;
                b[k] = r;
            }
        }
    } else {
        b[1] = 1;
        for (k = 2; k <= N; k++) {
            seq_t v = b[k - 1] * 4;
            v = v * (seq_t)(2 * k - 1);
            v = v * (seq_t)(2 * k + 1);
            v = v / (seq_t)(k + 1);
            v = v / (seq_t)(k + 2);
            b[k] = v;
        }
    }

    /* D = 1 + 2B, C = x*D^2 */
    d[0] = 1;
    for (k = 1; k <= N; k++) d[k] = b[k] * 2;
    c[0] = 0;
    for (k = 1; k <= N; k++) {
        seq_t s = 0;
        for (i = 0; i <= k - 1; i++) s += d[i] * d[k - 1 - i];
        c[k] = s;
    }

    for (k = 0; k <= N; k++) { S[k] = b[k]; P[k] = c[k]; }

    for (k = 1; k <= N; k++) {
        seq_t ak = S[k];
        a[k] = ak;
        for (j = k; j <= N; j++) S[j] -= ak * P[j];
        if (k < N) {
            seq_t *tmp;
            for (j = 0; j <= N; j++) T[j] = 0;
            for (j = k + 1; j <= N; j++) {
                seq_t s = 0;
                for (i = k; i <= j - 1; i++) s += P[i] * c[j - i];
                T[j] = s;
            }
            tmp = P; P = T; T = tmp;
        }
    }

    free(b); free(d); free(c); free(S); free(P); free(T);
    if (A_memo) free(A_memo);
    A_memo = a;
    A_N = N;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (n > A_N) {
        idx_t N = A_N * 2;
        if (N < n) N = n;
        if (N < 32) N = 32;
        build(N);
    }
    return A_memo[n];
}

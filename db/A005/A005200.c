/*
 * A005200  Total number of fixed points in rooted trees with n nodes.
 * https://oeis.org/A005200
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f. A(x) = T(x)*(1 + A(x) - A(x^2)), where T(x) = Sum t(k) x^k
 *            is the g.f. of A000081: t(1) = 1, t(m) = (1/(m-1)) *
 *            Sum_{k=1..m-1} (Sum_{d|k} d*t(d)) * t(m-k); hence a(m) =
 *            Sum_{k=1..m} t(k)*b(m-k), b(0) = 1, b(j) = a(j) - [j
 *            even]*a(j/2)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 23, u64 n <= 43, u128 n <= 85,
 *            big any n
 * Helpers:   grow_zero, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"

static seq_t *T = NULL;   /* t(k), A000081 */
static seq_t *S = NULL;   /* Sum_{d|k} d*t(d) */
static seq_t *A = NULL;   /* a(k) */
static idx_t N = 0;       /* computed indices 1..N */
static idx_t cap = 0;

/* exact division of S by m (S is a multiple of m); fixed-width safe */
static seq_t exact_div(seq_t s, idx_t m) {
    if (seq_is_big()) return s / (seq_t)m;
    int e = 0;
    idx_t o = m;
    while ((o & 1) == 0) { o >>= 1; e++; }
    unsigned __int128 inv = o;
    for (int i = 0; i < 7; i++) inv *= (unsigned __int128)2 - (unsigned __int128)o * inv;
    int w = (int)(sizeof(seq_t) * 8);
    if (e >= w) return 0;
    seq_t r = (s >> e) * u128_to_seq(inv);
    if (e > 0) r &= (((seq_t)1 << (w - e)) - 1);
    return r;
}

static void extend(idx_t n) {
    if (n <= N) return;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 64;
        while (nc < n + 1) nc *= 2;
        T = grow_zero(T, cap, nc);
        S = grow_zero(S, cap, nc);
        A = grow_zero(A, cap, nc);
        cap = nc;
    }
    for (idx_t m = N + 1; m <= n; m++) {
        /* t(m) */
        if (m == 1) {
            T[1] = 1;
        } else {
            seq_t sum = 0;
            for (idx_t k = 1; k <= m - 1; k++) sum += S[k] * T[m - k];
            T[m] = exact_div(sum, m - 1);
        }
        /* s(m) */
        {
            seq_t s = 0;
            for (idx_t d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t q = m / d;
                    s += (seq_t)d * T[d];
                    if (q != d) s += (seq_t)q * T[q];
                }
            }
            S[m] = s;
        }
        /* a(m) */
        {
            seq_t sum = 0;
            for (idx_t k = 1; k <= m; k++) {
                idx_t j = m - k;
                seq_t b;
                if (j == 0) b = 1;
                else if (j % 2 == 1) b = A[j];
                else b = A[j] - A[j / 2];
                sum += T[k] * b;
            }
            A[m] = sum;
        }
    }
    N = n;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    extend(n);
    return A[n];
}

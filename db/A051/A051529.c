/*
 * A051529  INVERT transform of A000081 = [1, 1, 1, 2, 4, 9, 20, 48, 115,
 *          286,...].
 * https://oeis.org/A051529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = b(n+1) where b(0)=1, b(m) = Sum_{k=1..m} c(k)*b(m-k),
 *            with c(1)=1 and c(k)=A000081(k-1) for k>=2 (INVERT transform of
 *            1,1,1,2,4,9,20,...)
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 23, u64 n <= 44, u128 n <= 86,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 635 (2026-11-07)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"
#include "../utils/mem.h"

#define A81_MAX 32768

static seq_t A81_T[A81_MAX + 2];
static seq_t A81_P[A81_MAX + 2];
static seq_t A81_M[A81_MAX + 2];
static idx_t A81_cap = 0;

static void a81_build(idx_t N)
{
    idx_t L = N;
    seq_t *P = A81_P;
    seq_t *M = A81_M;
    seq_t *T = A81_T;
    idx_t i, k, m, j;

    for (i = 0; i <= L; i++) { P[i] = 0; M[i] = 0; }
    for (i = 0; i <= N + 1; i++) T[i] = 0;
    P[0] = 1;
    T[0] = 0;
    T[1] = 1;

    for (k = 1; k < N; k++) {
        seq_t c = T[k];
        idx_t J = (L - 1) / k;
        M[0] = 1;
        for (j = 1; j <= J; j++) {
            unsigned long long r = (unsigned long long)(M[j - 1] % (seq_t)j);
            unsigned long long g = gcd_u64(r, (unsigned long long)j);
            seq_t x = M[j - 1] / (seq_t)g;
            seq_t y = (c + (seq_t)(j - 1)) / (seq_t)((unsigned long long)j / g);
            M[j] = x * y;
        }
        for (m = L - 1; m >= k; m--) {
            seq_t sum = P[m];
            idx_t r = m - k;
            for (j = 1;; j++) {
                sum += M[j] * P[r];
                if (r < k) break;
                r -= k;
            }
            P[m] = sum;
        }
        T[k + 1] = P[k];
    }
}

static seq_t A000081_a(idx_t n)
{
    if (n == 0) return 0;
    if (n > A81_MAX) n = A81_MAX;
    if (n > A81_cap) {
        idx_t nc = A81_cap + A81_cap / 2;
        if (nc < n) nc = n;
        if (nc < 16) nc = 16;
        if (nc > A81_MAX) nc = A81_MAX;
        a81_build(nc);
        A81_cap = nc;
    }
    return A81_T[n];
}

seq_t a(idx_t n)
{
    idx_t M = n + 1, m, k;
    seq_t *c = mem_new(seq_t, M + 1);
    seq_t *b = mem_new(seq_t, M + 1);
    seq_t res;

    c[1] = 1;
    for (k = 2; k <= M; k++) c[k] = A000081_a(k - 1);
    b[0] = 1;
    for (m = 1; m <= M; m++) {
        seq_t s = 0;
        for (k = 1; k <= m; k++) s += c[k] * b[m - k];
        b[m] = s;
    }
    res = b[M];
    mem_free(c);
    mem_free(b);
    return res;
}

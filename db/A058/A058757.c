/*
 * A058757  a(n) = n*coefficient of x^n in log(1+S), S = g.f. for A000084.
 * https://oeis.org/A058757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*A000084(n) - Sum_{k=1..n-1} a(k)*A000084(n-k), from
 *            n*[x^n] log(1+S) with S = Sum_{k>=1} A000084(k)*x^k (a(1) = 1)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Validated: partial, pass, 26 terms through n = 26 (2026-11-07)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"
#include "../utils/mem.h"

static seq_t A000084_small(idx_t n)
{
    seq_t P[130];
    seq_t C[130];
    idx_t i, k, j, m;
    for (i = 0; i <= n; i++)
        P[i] = 0;
    P[0] = 1;
    for (k = 1; k <= n; k++) {
        seq_t b = (k == 1) ? (seq_t)1 : P[k];
        idx_t J = n / k;
        C[0] = 1;
        for (j = 1; j <= J; j++) {
            unsigned long long r = (unsigned long long)(C[j - 1] % (seq_t)j);
            unsigned long long g = gcd_u64(r, (unsigned long long)j);
            seq_t num = b + (seq_t)(j - 1);
            C[j] = (C[j - 1] / (seq_t)g) * (num / (seq_t)(j / g));
        }
        for (m = n; m >= k; m--) {
            seq_t add = 0;
            for (j = 1; j * k <= m; j++)
                add += C[j] * P[m - j * k];
            P[m] += add;
        }
    }
    return P[n];
}

#define A84_MAXN 30000

static seq_t A000084_A[A84_MAXN + 1];
static seq_t A000084_B[A84_MAXN + 1];
static seq_t A000084_D[A84_MAXN + 1];
static idx_t A000084_top = 0;

static seq_t A000084_big(idx_t n)
{
    if (A000084_top == 0) {
        A000084_A[0] = 1;
        A000084_A[1] = 1;
        A000084_B[1] = 1;
        A000084_D[1] = 1;
        A000084_top = 1;
    }
    while (A000084_top < n) {
        idx_t m = A000084_top + 1;
        seq_t s = 0;
        seq_t t;
        idx_t q, k;
        for (q = 1; q <= m / 2; q++)
            if (m % q == 0)
                s += A000084_B[q] * (seq_t)q;
        t = s;
        for (k = 1; k < m; k++)
            t += A000084_D[k] * A000084_A[m - k];
        A000084_A[m] = t * 2 / (seq_t)m;
        A000084_B[m] = A000084_A[m] / 2;
        A000084_D[m] = s + A000084_B[m] * (seq_t)m;
        A000084_top = m;
    }
    return A000084_A[n];
}

static int A058757_wide(void)
{
    seq_t x = 1;
    int w = 0;
    while (w < 300 && x != 0) {
        x = x * 2;
        w++;
    }
    return w >= 300;
}

static seq_t A000084_a(idx_t n, int wide)
{
    if (n == 0)
        return 1;
    if (!wide) {
        if (n > 128)
            return 0;
        return A000084_small(n);
    }
    return A000084_big(n);
}

static seq_t *A058757_L = NULL;
static seq_t *A058757_F = NULL;
static idx_t A058757_have = 0;
static idx_t A058757_cap = 0;

seq_t a(idx_t n)
{
    int wide = A058757_wide();
    idx_t m, k;
    if (n == 0)
        return 0;
    if (!wide && n > 130)
        return 0;
    if (n >= A058757_cap) {
        idx_t nc = A058757_cap * 2;
        seq_t *nl, *nf;
        if (nc < 64)
            nc = 64;
        if (nc <= n)
            nc = n + 1;
        nl = mem_new(seq_t, (size_t)nc);
        nf = mem_new(seq_t, (size_t)nc);
        if (A058757_L) {
            mem_copy(nl, A058757_L, (size_t)(A058757_have + 1) * sizeof(seq_t));
            mem_copy(nf, A058757_F, (size_t)(A058757_have + 1) * sizeof(seq_t));
            mem_free(A058757_L);
            mem_free(A058757_F);
        }
        A058757_L = nl;
        A058757_F = nf;
        A058757_cap = nc;
    }
    for (m = A058757_have + 1; m <= n; m++) {
        seq_t s;
        A058757_F[m] = A000084_a(m, wide);
        s = (seq_t)m * A058757_F[m];
        for (k = 1; k < m; k++)
            s -= A058757_L[k] * A058757_F[m - k];
        A058757_L[m] = s;
        A058757_have = m;
    }
    return A058757_L[n];
}

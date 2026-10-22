/*
 * A000875  Number of switching networks with S(n) and C(2,2) acting on the
 *          domain and GL(3,2) acting on the range.
 * https://oeis.org/A000875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside count over the hyperoctahedral group B_n acting on
 *            {0,1}^n and GL(3,2) acting on F_2^3: a(n) = (1/(2^n*n!*168)) *
 *            Sum_{g in B_n} Sum_{h in GL(3,2)} 2^(Sum_{cycles c of g on
 *            {0,1}^n} dim Fix(h^len(c)))
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   a875_combine, a875_cycle, a875_dimfix, a875_fix, a875_from_u128,
 *            a875_gcd, a875_mu, a875_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a875_combine.h"
#include "../utils/a875_cycle.h"
#include "../utils/a875_dimfix.h"
#include "../utils/a875_fix.h"
#include "../utils/a875_from_u128.h"
#include "../utils/a875_gcd.h"
#include "../utils/a875_mu.h"
#include "../utils/a875_width.h"

static int *a875_mm = 0, *a875_nn = 0;
static unsigned __int128 a875_small_total;
static seq_t a875_big_total;
static int a875_small;

static const uint64_t a875_hsz[5] = {1, 21, 56, 42, 48};

static void a875_leaf(idx_t n)
{
    uint64_t *L = (uint64_t *)malloc(sizeof(uint64_t));
    uint64_t *C = (uint64_t *)malloc(sizeof(uint64_t));
    int sz = 1, i, h;
    idx_t k, ncyc = 0;
    long long E[5] = {0, 0, 0, 0, 0};
    L[0] = 1;
    C[0] = 1;
    for (k = 1; k <= n; k++) {
        int s;
        ncyc += (idx_t)(a875_mm[k] + a875_nn[k]);
        for (s = 0; s < 2; s++) {
            int cnt = s ? a875_nn[k] : a875_mm[k], r;
            uint64_t *L2, *C2;
            int sz2;
            if (cnt == 0) continue;
            sz2 = a875_cycle(k, s, &L2, &C2);
            for (r = 0; r < cnt; r++) a875_combine(&L, &C, &sz, L2, C2, sz2);
            free(L2);
            free(C2);
        }
    }
    for (i = 0; i < sz; i++)
        for (h = 0; h < 5; h++) E[h] += (long long)C[i] * a875_dimfix(h, L[i]);
    free(L);
    free(C);

    if (a875_small) {
        unsigned __int128 cs = 1, S = 0;
        idx_t j;
        int r;
        for (j = 2; j <= n; j++) cs *= j;
        for (k = 1; k <= n; k++) {
            for (r = 0; r < a875_mm[k] + a875_nn[k]; r++) cs /= k;
            for (r = 2; r <= a875_mm[k]; r++) cs /= (unsigned)r;
            for (r = 2; r <= a875_nn[k]; r++) cs /= (unsigned)r;
        }
        cs <<= (n - ncyc);
        for (h = 0; h < 5; h++) S += ((unsigned __int128)a875_hsz[h]) << E[h];
        a875_small_total += cs * S;
    } else {
        seq_t cs = 1, S = 0;
        idx_t j;
        int r;
        for (j = 2; j <= n; j++) cs *= (seq_t)j;
        for (k = 1; k <= n; k++) {
            for (r = 0; r < a875_mm[k] + a875_nn[k]; r++) cs /= (seq_t)k;
            for (r = 2; r <= a875_mm[k]; r++) cs /= (seq_t)r;
            for (r = 2; r <= a875_nn[k]; r++) cs /= (seq_t)r;
        }
        for (j = 0; j < n - ncyc; j++) cs *= 2;
        for (h = 0; h < 5; h++) S += ((seq_t)1 << E[h]) * (seq_t)a875_hsz[h];
        a875_big_total += cs * S;
    }
}

static void a875_rec(idx_t k, idx_t rem, idx_t n)
{
    idx_t c;
    int m;
    if (rem == 0) { a875_leaf(n); return; }
    if (k > rem) return;
    for (c = 0; c * k <= rem; c++)
        for (m = 0; m <= (int)c; m++) {
            a875_mm[k] = m;
            a875_nn[k] = (int)c - m;
            a875_rec(k + 1, rem - c * k, n);
        }
    a875_mm[k] = 0;
    a875_nn[k] = 0;
}

seq_t a(idx_t n)
{
    idx_t j;
    seq_t res;
    a875_mm = (int *)calloc(n + 2, sizeof(int));
    a875_nn = (int *)calloc(n + 2, sizeof(int));
    a875_small = (n <= 5);
    a875_small_total = 0;
    a875_big_total = 0;
    a875_rec(1, n, n);
    if (a875_small) {
        unsigned __int128 Z = 168;
        for (j = 1; j <= n; j++) Z *= 2 * (unsigned __int128)j;
        res = a875_from_u128(a875_small_total / Z);
    } else {
        seq_t Z = 168;
        for (j = 1; j <= n; j++) Z *= (seq_t)2 * (seq_t)j;
        res = a875_big_total / Z;
    }
    free(a875_mm);
    free(a875_nn);
    a875_mm = 0;
    a875_nn = 0;
    return res;
}

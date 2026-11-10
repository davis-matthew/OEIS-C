/*
 * A050327  Number of factorizations into distinct squarefree factors indexed
 *          by prime signatures. A050326(A025487).
 * https://oeis.org/A050327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A050326(A025487(n)); A025487(n) is the n-th least number
 *            of its prime signature (products of primorials), A050326(m) =
 *            number of factorizations of m into distinct squarefree factors >
 *            1. Computed by enumerating exponent signatures sorted by
 *            log-size, then a 0/1 knapsack over divisor-expone...
 * Range:     u8 n <= 139, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 99 terms through n = 497 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "../utils/mem.h"

#define A050327_K 16

typedef struct {
    double lg;
    unsigned char e[A050327_K];
} A050327_sig;

static const double A050327_p[A050327_K] = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53};
static double A050327_lp[A050327_K];
static int A050327_lpinit = 0;

static A050327_sig *A050327_list = NULL;
static size_t A050327_cnt = 0, A050327_cap = 0;
static double A050327_L = 0.0;

static void A050327_push(double lg, const unsigned char *e)
{
    if (A050327_cnt == A050327_cap) {
        size_t nc = A050327_cap ? A050327_cap * 2 : 1024;
        A050327_sig *np = (A050327_sig *)realloc(A050327_list, nc * sizeof(A050327_sig));
        A050327_list = np;
        A050327_cap = nc;
    }
    A050327_sig *s = &A050327_list[A050327_cnt++];
    s->lg = lg;
    memcpy(s->e, e, A050327_K);
}

static void A050327_rec(int idx, int maxe, double lg, unsigned char *e)
{
    int x;
    A050327_push(lg, e);
    if (idx >= A050327_K) return;
    for (x = 1; x <= maxe; x++) {
        double nl = lg + x * A050327_lp[idx];
        if (nl > A050327_L + 1e-12) break;
        e[idx] = (unsigned char)x;
        A050327_rec(idx + 1, x, nl, e);
    }
    e[idx] = 0;
}

static int A050327_cmp(const void *a, const void *b)
{
    double x = ((const A050327_sig *)a)->lg, y = ((const A050327_sig *)b)->lg;
    return (x < y) ? -1 : (x > y) ? 1 : 0;
}

static void A050327_build(size_t need)
{
    int i;
    if (!A050327_lpinit) {
        for (i = 0; i < A050327_K; i++) A050327_lp[i] = log(A050327_p[i]);
        A050327_lpinit = 1;
    }
    while (A050327_cnt < need) {
        unsigned char e[A050327_K];
        A050327_L = (A050327_L == 0.0) ? 10.0 : A050327_L + 3.0;
        A050327_cnt = 0;
        memset(e, 0, sizeof(e));
        A050327_rec(0, 1000, 0.0, e);
        qsort(A050327_list, A050327_cnt, sizeof(A050327_sig), A050327_cmp);
    }
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)(n - 1);
    int k = 0, i;
    unsigned e[A050327_K];
    size_t stride[A050327_K];
    size_t D = 1, st, s, top;
    unsigned *nz;
    seq_t *dp;
    seq_t res;

    A050327_build(idx + 1);
    for (i = 0; i < A050327_K; i++) {
        if (A050327_list[idx].e[i]) { e[k++] = A050327_list[idx].e[i]; }
    }
    if (k == 0) return 1;
    for (i = 0; i < k; i++) { stride[i] = D; D *= (size_t)(e[i] + 1); }

    nz = (unsigned *)malloc(D * sizeof(unsigned));
    for (st = 0; st < D; st++) {
        unsigned m = 0;
        for (i = 0; i < k; i++) {
            if ((st / stride[i]) % (size_t)(e[i] + 1) >= 1) m |= (1u << i);
        }
        nz[st] = m;
    }
    dp = mem_new(seq_t, D);
    dp[0] = 1;
    top = D - 1;
    for (s = 1; s < ((size_t)1 << k); s++) {
        size_t delta = 0;
        for (i = 0; i < k; i++) if (s & ((size_t)1 << i)) delta += stride[i];
        for (st = top; st >= delta && st > 0; st--) {
            if ((((unsigned)s) & ~nz[st]) == 0) dp[st] += dp[st - delta];
        }
    }
    res = dp[top];
    mem_free(dp);
    free(nz);
    return res;
}

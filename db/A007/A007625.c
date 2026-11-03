/*
 * A007625  Number of M-sequences from multicomplexes on at most 6 variables
 *          with no monomial of degree more than n-1.
 * https://oeis.org/A007625
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(-1) = 1; for n >= 0, a(n) = 1 + (number of M-sequences
 *            (1,h_1,...,h_n) with h_1 <= 6), where each step must satisfy
 *            Macaulay's bound h_{i+1} <= h_i^<i>. Computed by dynamic
 *            programming over the value of h_i, using prefix sums, from
 *            degree n down to degree 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 11, u128 n <= 13,
 *            big any n
 * Validated: partial, pass, 15 terms through n = 13 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

/* C(t+d, d) for small d */
static unsigned long long cb(long long t, int d)
{
    unsigned long long r = 1;
    int j;
    for (j = 1; j <= d; j++)
        r = r * (unsigned long long)(t + j) / (unsigned long long)j;
    return r;
}

/* Macaulay upper bound v^<i>; at degree i we have v <= C(i+5,5) */
static unsigned long long mac_up(unsigned long long v, long long i)
{
    unsigned long long res = 0;
    long long t = i;
    int d = 5;
    while (v > 0 && t >= 1) {
        while (d > 0 && cb(t, d) > v) d--;
        v -= cb(t, d);
        res += cb(t + 1, d);
        t--;
    }
    return res;
}

#define MEMO_N 128
static seq_t memo[MEMO_N];
static bool memo_ok[MEMO_N];

seq_t a(idx_t n)
{
    seq_t *P, *Q, res;
    long long M, Mi, v, i;
    if (n < 0) return (seq_t)1;
    if (n == 0) return (seq_t)2;
    if (n < MEMO_N && memo_ok[n]) return memo[n];

    M = (long long)cb(n, 5);
    P = alloc_zero((idx_t)(M + 1));
    for (v = 0; v <= M; v++)
        P[v] = (seq_t)(idx_t)v + 1;
    for (i = n - 1; i >= 1; i--) {
        seq_t acc = 0;
        Mi = (long long)cb(i, 5);
        Q = alloc_zero((idx_t)(Mi + 1));
        for (v = 0; v <= Mi; v++) {
            acc += P[(long long)mac_up((unsigned long long)v, i)];
            Q[v] = acc;
        }
        release(P, (idx_t)(M + 1));
        P = Q;
        M = Mi;
    }
    res = P[6] + 1;
    release(P, (idx_t)(M + 1));
    if (n < MEMO_N) { memo[n] = res; memo_ok[n] = true; }
    return res;
}

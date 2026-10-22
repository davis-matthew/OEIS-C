/*
 * A006339  Least hypotenuse of n distinct Pythagorean triangles.
 * https://oeis.org/A006339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   if 2n+1 = f_1*f_2*...*f_k with f_1 >= f_2 >= ... >= f_k >= 3
 *            odd, a(n) = min over all such factorizations of prod
 *            p_i^((f_i-1)/2), where p_1 = 5, p_2 = 13, p_3 = 17, ... are the
 *            primes == 1 (mod 4). (A hypotenuse c has (d(c')-1)/2 Pythagorean
 *            triangles, c' = part of c made of primes == 1 mod 4.)...
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 28, u128 n <= 55,
 *            big any n
 * Helpers:   powseq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdbool.h>
#include "../utils/powseq.h"

#define NP 40

static long long pr[NP];
static double lp[NP];
static int pr_ready = 0;

static int best_len;
static long long best_f[NP];
static long long cur_f[NP];
static double best_log;

static void init_primes(void) {
    int cnt = 0;
    long long c;
    if (pr_ready) return;
    for (c = 5; cnt < NP; c += 4) {
        long long d;
        bool ok = true;
        for (d = 2; d * d <= c; d++)
            if (c % d == 0) { ok = false; break; }
        if (ok) { pr[cnt] = c; lp[cnt] = log((double)c); cnt++; }
    }
    pr_ready = 1;
}

static void rec(long long m, long long maxf, int depth, double lg) {
    long long f;
    if (lg >= best_log) return;
    if (m == 1) {
        int i;
        best_log = lg;
        best_len = depth;
        for (i = 0; i < depth; i++) best_f[i] = cur_f[i];
        return;
    }
    if (depth >= NP) return;
    for (f = 3; f <= maxf && f <= m; f += 2) {
        if (m % f != 0) continue;
        cur_f[depth] = f;
        rec(m / f, f, depth + 1, lg + (double)((f - 1) / 2) * lp[depth]);
    }
}

seq_t a(idx_t n) {
    long long m = 2 * (long long)n + 1;
    seq_t r = 1;
    int i;
    init_primes();
    best_log = HUGE_VAL;
    best_len = 0;
    rec(m, m, 0, 0.0);
    for (i = 0; i < best_len; i++)
        r *= powseq((seq_t)pr[i], (best_f[i] - 1) / 2);
    return r;
}

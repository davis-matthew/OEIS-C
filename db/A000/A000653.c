/*
 * A000653  Invertible Boolean functions of n variables.
 * https://oeis.org/A000653
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside over S_n x S_n acting on bijections f of {0,1}^n by f
 *            -> P_s o f o P_t: a(n) = (1/n!^2) * Sum_{lambda partition of n}
 *            (n!/z(lambda))^2 * Prod_k k^c_k * c_k!, where z(lambda) = Prod_m
 *            m^{a_m} a_m! and c_k = number of k-cycles that a permutation of
 *            cycle type lambda induces on {0,1}^n: k*c...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   p653_gcd, p653_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/p653_gcd.h"
#include "../utils/p653_pow.h"

static int p653_parts[64];
static seq_t p653_total;
static seq_t p653_nfact;

static void p653_process(int m)
{
    unsigned long long L = 1;
    int i;
    for (i = 0; i < m; i++)
        L = L / p653_gcd(L, (unsigned long long)p653_parts[i]) * (unsigned long long)p653_parts[i];

    long long *p = (long long *)malloc((size_t)(L + 1) * sizeof(long long));
    unsigned long long k, d;
    for (k = 1; k <= L; k++) {
        if (L % k) { p[k] = 0; continue; }
        unsigned long long e = 0;
        for (i = 0; i < m; i++) e += p653_gcd(k, (unsigned long long)p653_parts[i]);
        long long v = (long long)(1ULL << e);
        for (d = 1; d < k; d++)
            if (k % d == 0) v -= p[d];
        p[k] = v;
    }

    seq_t Z = 1;
    for (k = 1; k <= L; k++) {
        if (p[k] <= 0) continue;
        unsigned long long c = (unsigned long long)p[k] / k;
        Z *= p653_pow((seq_t)k, c);
        unsigned long long j;
        for (j = 2; j <= c; j++) Z *= (seq_t)j;
    }
    free(p);

    seq_t z = 1;
    i = 0;
    while (i < m) {
        int val = p653_parts[i];
        int a = 0;
        while (i < m && p653_parts[i] == val) { a++; i++; }
        z *= p653_pow((seq_t)val, (unsigned long long)a);
        int j;
        for (j = 2; j <= a; j++) z *= (seq_t)j;
    }

    seq_t cls = p653_nfact / z;
    p653_total += cls * cls * Z;
}

static void p653_gen(int rem, int maxp, int m)
{
    if (rem == 0) { p653_process(m); return; }
    int q;
    for (q = (rem < maxp ? rem : maxp); q >= 1; q--) {
        p653_parts[m] = q;
        p653_gen(rem - q, q, m + 1);
    }
}

seq_t a(idx_t n)
{
    int nn = (int)n;
    idx_t i;
    p653_nfact = 1;
    for (i = 2; i <= n; i++) p653_nfact *= (seq_t)i;
    p653_total = 0;
    p653_gen(nn, nn, 0);
    return p653_total / (p653_nfact * p653_nfact);
}

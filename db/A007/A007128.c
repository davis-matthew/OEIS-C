/*
 * A007128  Number of unlabeled strength 3 Eulerian graphs with n nodes.
 * https://oeis.org/A007128
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{partitions p of n} 2^(2*N(p) - k(p) + [p has an odd
 *            part]) / z(p), where k(p) = number of parts, N(p) = Sum_i
 *            floor(p_i/2) + Sum_{i<j} gcd(p_i,p_j) (number of edge orbits),
 *            z(p) = Prod_c c^m_c * m_c! (Polya / Burnside count: edge weights
 *            0..3 give 2 choices per parity, and the parity...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 7, u64 n <= 10, u128 n <= 13,
 *            big any n
 * Helpers:   v2l
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/v2l.h"

static int g_big;
static int g_bits;
static seq_t g_S;
static seq_t g_F;
static long *g_parts;
static long *g_mults;

static long g_gcd(long a, long b)
{
    while (b) { long t = a % b; a = b; b = t; }
    return a;
}

static seq_t inv_odd(seq_t w)
{
    seq_t x = w;
    int i;
    for (i = 0; i < 8; i++) x = x * ((seq_t)2 - w * x);
    return x;
}

/* zw = z (big mode) or odd part of z mod 2^bits (fixed mode); s = v2(z) */
static void rec(long rem, long maxp, long long N, long k, int odd,
                seq_t zw, long long s, int depth)
{
    long c;
    if (rem == 0) {
        long long e = 2 * N - k + odd;
        if (g_big) {
            g_S += (g_F / zw) << e;
        } else {
            long long sh = e - s;
            if (sh < g_bits) g_S += inv_odd(zw) << sh;
        }
        return;
    }
    for (c = (rem < maxp ? rem : maxp); c >= 1; c--) {
        long long gsum = 0;
        int d;
        long m;
        seq_t z2 = zw;
        long long s2 = s;
        for (d = 0; d < depth; d++) gsum += (long long)g_mults[d] * g_gcd(c, g_parts[d]);
        for (m = 1; m * c <= rem; m++) {
            long long N2 = N + (long long)m * (c / 2) + (long long)m * (m - 1) / 2 * c
                           + (long long)m * gsum;
            if (g_big) {
                z2 *= (seq_t)c;
                z2 *= (seq_t)m;
            } else {
                int vc = v2l(c), vm = v2l(m);
                s2 += vc + vm;
                z2 *= (seq_t)(c >> vc);
                z2 *= (seq_t)(m >> vm);
            }
            g_parts[depth] = c;
            g_mults[depth] = m;
            rec(rem - m * c, c - 1, N2, k + m, odd || (c & 1), z2, s2, depth + 1);
        }
    }
}

seq_t a(idx_t n)
{
    seq_t x = 1;
    int bits = 0;
    idx_t i;
    while (bits < 300 && x != 0) { x <<= 1; bits++; }
    g_big = (x != 0);
    g_bits = bits;

    g_parts = (long *)malloc(sizeof(long) * (size_t)(n + 1));
    g_mults = (long *)malloc(sizeof(long) * (size_t)(n + 1));
    g_S = 0;
    g_F = 1;
    if (g_big) {
        for (i = 2; i <= n; i++) g_F *= (seq_t)i;
    }
    rec((long)n, (long)n, 0, 0, 0, (seq_t)1, 0, 0);
    free(g_parts);
    free(g_mults);
    if (g_big) return g_S / g_F;
    return g_S;
}

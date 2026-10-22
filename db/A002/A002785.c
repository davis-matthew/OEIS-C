/*
 * A002785  Number of self-complementary oriented graphs with n nodes.
 * https://oeis.org/A002785
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(2m) = Sum_{j partition of m into odd parts} Product_k
 *            2^(k*jk^2-jk) * Product_{r<t} 2^(2*gcd(r,t)*jr*jt) / Product_k
 *            k^jk*jk!; a(2m+1) = Sum_{j partition of m into odd parts}
 *            Product_{r,t} 2^(gcd(r,t)*jr*jt) / Product_k k^jk*jk!. Big
 *            build: sum 2^E*(m!/z_j), then divide by m!. Fixed build: each...
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 14, u64 n <= 19, u128 n <= 26,
 *            big any n
 * Helpers:   a2785_gcd, a2785_inv
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2785_gcd.h"
#include "../utils/a2785_inv.h"

static int g_big;
static int g_W;
static int g_even;
static long long *g_pk;
static long long *g_pc;
static int g_depth;
static seq_t g_sum;
static seq_t g_mfact;

static void a2785_leaf(long long E, seq_t zbig, long long v, seq_t zodd) {
    if (g_big) {
        seq_t p = 1;
        p <<= (idx_t)E;
        g_sum += p * (g_mfact / zbig);
    } else {
        long long f = E - v;
        if (f < g_W) {
            seq_t p = 1;
            p <<= (idx_t)f;
            g_sum += p * a2785_inv(zodd);
        }
    }
}

static void a2785_rec(long long rem, long long maxk, long long E,
                      seq_t zbig, long long v, seq_t zodd) {
    if (rem == 0) {
        a2785_leaf(E, zbig, v, zodd);
        return;
    }
    long long k = maxk < rem ? maxk : rem;
    if (k % 2 == 0) k--;
    for (; k >= 1; k -= 2) {
        long long cross = 0;
        for (int i = 0; i < g_depth; i++)
            cross += 2 * a2785_gcd(g_pk[i], k) * g_pc[i];
        seq_t zb = zbig, zo = zodd;
        long long vv = v;
        for (long long c = 1; c * k <= rem; c++) {
            if (g_big) {
                zb *= (seq_t)(idx_t)k;
                zb *= (seq_t)(idx_t)c;
            } else {
                long long cc = c;
                while (cc % 2 == 0) { cc /= 2; vv++; }
                zo *= (seq_t)(idx_t)k;
                zo *= (seq_t)(idx_t)cc;
            }
            long long selfE = g_even ? k * c * c - c : k * c * c;
            long long addE = selfE + cross * c;
            g_pk[g_depth] = k;
            g_pc[g_depth] = c;
            g_depth++;
            a2785_rec(rem - c * k, k - 2, E + addE, zb, vv, zo);
            g_depth--;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    {
        seq_t x = 1;
        int w = 0;
        while (x != 0 && w < 300) { x *= 2; w++; }
        g_big = (w >= 300);
        g_W = w;
    }
    long long m = (long long)(n / 2);
    g_even = (n % 2 == 0);
    g_pk = (long long *)malloc(sizeof(long long) * (size_t)(m + 2));
    g_pc = (long long *)malloc(sizeof(long long) * (size_t)(m + 2));
    g_depth = 0;
    g_sum = 0;
    g_mfact = 1;
    if (g_big) {
        for (long long i = 2; i <= m; i++) g_mfact *= (seq_t)(idx_t)i;
    }
    long long start = (m % 2 == 1) ? m : m - 1;
    a2785_rec(m, start, 0, (seq_t)1, 0, (seq_t)1);
    free(g_pk);
    free(g_pc);
    seq_t r = g_sum;
    if (g_big) r = r / g_mfact;
    return r;
}

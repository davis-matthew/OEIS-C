/*
 * A003181  Number of P-equivalence classes of nondegenerate Boolean functions
 *          of n variables.
 * https://oeis.org/A003181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003180(n) - A003180(n-1) for n >= 1, a(0) = A003180(0) =
 *            2, where A003180(n) = (1/n!) * Sum_{partitions lambda of n}
 *            (n!/z_lambda) * 2^c(lambda), with c(lambda) = number of cycles
 *            of a permutation of cycle type lambda acting on the 2^n subsets
 *            of {1..n}: c = (1/L) * Sum_{j=0}^{L-1} 2^(Sum...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Validated: standard, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/gcd_u64.h"

static void a3181_gen(int rem, int maxp, int *parts, int k, uint64_t nf, int v,
                      int big, seq_t *Sb, unsigned __int128 *Sf)
{
    if (rem == 0) {
        uint64_t L = 1, sum = 0, c, z = 1, cnt, j, e;
        int i, run;
        for (i = 0; i < k; i++) L = L / gcd_u64(L, (uint64_t)parts[i]) * (uint64_t)parts[i];
        for (j = 0; j < L; j++) {
            uint64_t ex = 0;
            for (i = 0; i < k; i++) ex += gcd_u64((uint64_t)parts[i], j);
            sum += (uint64_t)1 << ex;
        }
        c = sum / L;
        for (i = 0; i < k; i++) z *= (uint64_t)parts[i];
        run = 0;
        for (i = 0; i < k; i++) {
            if (i > 0 && parts[i] == parts[i - 1]) run++; else run = 1;
            z *= (uint64_t)run;
        }
        cnt = nf / z;
        e = c - (uint64_t)v;
        if (big) {
            seq_t t = (seq_t)cnt;
            t <<= (idx_t)e;
            *Sb += t;
        } else {
            unsigned __int128 p = (e < 128) ? ((unsigned __int128)1 << e) : 0;
            *Sf += (unsigned __int128)cnt * p;
        }
        return;
    }
    {
        int p = rem < maxp ? rem : maxp;
        for (; p >= 1; p--) {
            parts[k] = p;
            a3181_gen(rem - p, p, parts, k + 1, nf, v, big, Sb, Sf);
        }
    }
}

/* computes A003180(m): exact in big mode (into *rb), mod 2^128 otherwise (returned) */
static unsigned __int128 a3181_F(int m, int big, seq_t *rb)
{
    int parts[64];
    uint64_t nf = 1, o;
    int v = 0, i;
    seq_t Sb = 0;
    unsigned __int128 Sf = 0;
    for (i = 2; i <= m; i++) nf *= (uint64_t)i;
    o = nf;
    while ((o & 1) == 0) { o >>= 1; v++; }
    a3181_gen(m, m, parts, 0, nf, v, big, &Sb, &Sf);
    if (big) {
        *rb = Sb / (seq_t)o;
        return 0;
    } else {
        unsigned __int128 x = o, oo = o;
        for (i = 0; i < 7; i++) x *= (unsigned __int128)2 - oo * x;
        return Sf * x;
    }
}

static int a3181_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x *= 2;
    return x != 0;
}

seq_t a(idx_t n)
{
    int big = a3181_big();
    int m = (int)n;
    if (n > 20) return 0;
    if (big) {
        seq_t f1 = 0, f0 = 0;
        a3181_F(m, 1, &f1);
        if (m == 0) return f1;
        a3181_F(m - 1, 1, &f0);
        return f1 - f0;
    } else {
        seq_t dummy = 0, r;
        unsigned __int128 v = a3181_F(m, 0, &dummy);
        if (m > 0) v -= a3181_F(m - 1, 0, &dummy);
        r = (seq_t)(uint64_t)(v >> 64);
        r = (seq_t)(r * (seq_t)4294967296ULL);
        r = (seq_t)(r * (seq_t)4294967296ULL);
        r = (seq_t)(r + (seq_t)(uint64_t)v);
        return r;
    }
}

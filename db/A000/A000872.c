/*
 * A000872  Number of switching networks with S(n) acting on the domain and
 *          GL(3,2) acting on the range.
 * https://oeis.org/A000872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(168*n!)) * Sum_{sigma in S(n)} Sum_{g in GL(3,2)}
 *            Prod_{cycles C of sigma on {0,1}^n} #{v in GF(2)^3 : g^|C| v =
 *            v} (Burnside's lemma). The sum runs over partitions of n. The
 *            number of cycles c_L of length L of sigma on {0,1}^n is c_L =
 *            (1/L) Sum_{d|L} mu(L/d) 2^(Sum_i gcd(d,lambda_i))...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 4, u128 n <= 5,
 *            big any n
 * Helpers:   m872_exps, m872_fixed, m872_from128, m872_gcd, m872_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/m872_exps.h"
#include "../utils/m872_fixed.h"
#include "../utils/m872_from128.h"
#include "../utils/m872_gcd.h"
#include "../utils/m872_mu.h"

static const unsigned m872_cnt[5] = {1, 21, 56, 42, 48};

static void m872_rec(int rem, int maxp, int *p, int k, int big,
                     unsigned long long nf64, seq_t nfbig,
                     unsigned __int128 *t128, seq_t *tbig)
{
    if (rem == 0) {
        unsigned long long E[5];
        int i, j;
        m872_exps(p, k, E);
        if (big) {
            seq_t z = 1, S = 0, cls;
            i = 0;
            while (i < k) {
                int r = 1;
                z *= (seq_t)p[i];
                j = i + 1;
                while (j < k && p[j] == p[i]) { z *= (seq_t)p[j]; r++; z *= (seq_t)r; j++; }
                i = j;
            }
            cls = nfbig / z;
            for (j = 0; j < 5; j++)
                S += (seq_t)m872_cnt[j] * (((seq_t)1) << (idx_t)E[j]);
            *tbig += S * cls;
        } else {
            unsigned long long z = 1;
            unsigned __int128 S = 0;
            i = 0;
            while (i < k) {
                unsigned long long r = 1;
                z *= (unsigned long long)p[i];
                j = i + 1;
                while (j < k && p[j] == p[i]) { z *= (unsigned long long)p[j]; r++; z *= r; j++; }
                i = j;
            }
            for (j = 0; j < 5; j++)
                S += (unsigned __int128)m872_cnt[j] * (((unsigned __int128)1) << E[j]);
            *t128 += S * (unsigned __int128)(nf64 / z);
        }
        return;
    }
    {
        int q;
        for (q = (maxp < rem ? maxp : rem); q >= 1; q--) {
            p[k] = q;
            m872_rec(rem - q, q, p, k + 1, big, nf64, nfbig, t128, tbig);
        }
    }
}

seq_t a(idx_t n)
{
    int *p;
    idx_t i;
    if (n < 1) return 0;
    p = (int *)malloc(sizeof(int) * (size_t)(n + 1));
    if (m872_fixed()) {
        unsigned long long nf = 1;
        unsigned __int128 T = 0;
        seq_t dummy = 0;
        if (n > 5) { free(p); return 0; }
        for (i = 2; i <= n; i++) nf *= i;
        m872_rec((int)n, (int)n, p, 0, 0, nf, dummy, &T, &dummy);
        free(p);
        return m872_from128(T / ((unsigned __int128)nf * 168));
    } else {
        seq_t nf = 1, T = 0;
        unsigned __int128 dummy = 0;
        for (i = 2; i <= n; i++) nf *= (seq_t)i;
        m872_rec((int)n, (int)n, p, 0, 1, 0, nf, &dummy, &T);
        free(p);
        return T / (nf * 168);
    }
}

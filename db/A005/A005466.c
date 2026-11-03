/*
 * A005466  5 X 5 stochastic matrices of integers.
 * https://oeis.org/A005466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   let H(r) = number of 5 X 5 nonnegative integer matrices with all
 *            row and column sums r (computed by a DP over rows), and h_k =
 *            Sum_{j=0..k} (-1)^j*C(17,j)*H(k-j) (h* vector of the Birkhoff
 *            polytope B_5, Sum_r H(r) x^r = h(x)/(1-x)^17). Then h(x) =
 *            Sum_{k=0..6} a(k) x^k (1-x)^(12-2k), i.e. a(k) =...
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: full, pass, 7 terms through n = 6 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

static seq_t from_i128(__int128 v)
{
    if (v >= -(__int128)4000000000000000000LL && v <= (__int128)4000000000000000000LL)
        return (seq_t)(long long)v;
    {
        __int128 q = v / ((__int128)1 << 32);
        long long rem = (long long)(v - q * ((__int128)1 << 32));
        return from_i128(q) * (seq_t)4294967296LL + (seq_t)rem;
    }
}

static long long cnt_H(int r)
{
    int B = r + 1, i, row;
    long long size = 1, s, target = 0, pw = 1, res;
    long long *cur, *nxt;
    int comps[2000][5];
    int nc = 0, c0, c1, c2, c3;
    for (i = 0; i < 5; i++) size *= B;
    for (c0 = 0; c0 <= r; c0++)
        for (c1 = 0; c0 + c1 <= r; c1++)
            for (c2 = 0; c0 + c1 + c2 <= r; c2++)
                for (c3 = 0; c0 + c1 + c2 + c3 <= r; c3++) {
                    comps[nc][0] = c0; comps[nc][1] = c1; comps[nc][2] = c2;
                    comps[nc][3] = c3; comps[nc][4] = r - c0 - c1 - c2 - c3;
                    nc++;
                }
    cur = (long long *)calloc((size_t)size, sizeof(long long));
    nxt = (long long *)calloc((size_t)size, sizeof(long long));
    cur[0] = 1;
    for (row = 0; row < 5; row++) {
        for (s = 0; s < size; s++) nxt[s] = 0;
        for (s = 0; s < size; s++) {
            int d[5], k;
            long long t = s;
            if (!cur[s]) continue;
            for (i = 0; i < 5; i++) { d[i] = (int)(t % B); t /= B; }
            for (k = 0; k < nc; k++) {
                long long idx = 0, p = 1;
                bool ok = true;
                for (i = 0; i < 5; i++) {
                    int v = d[i] + comps[k][i];
                    if (v > r) { ok = false; break; }
                    idx += v * p;
                    p *= B;
                }
                if (ok) nxt[idx] += cur[s];
            }
        }
        { long long *tmp = cur; cur = nxt; nxt = tmp; }
    }
    for (i = 0; i < 5; i++) { target += r * pw; pw *= B; }
    res = cur[target];
    free(cur);
    free(nxt);
    return res;
}

static long long binom_ll(int n, int k)
{
    long long r = 1;
    int i;
    if (k < 0 || k > n) return 0;
    for (i = 1; i <= k; i++) r = r * (n - k + i) / i;
    return r;
}

seq_t a(idx_t n)
{
    static bool done = false;
    static long long av[7];
    if (!done) {
        long long H[7], h[7];
        int k, j;
        for (k = 0; k <= 6; k++) H[k] = cnt_H(k);
        for (k = 0; k <= 6; k++) {
            long long s = 0;
            for (j = 0; j <= k; j++) {
                long long t = binom_ll(17, j) * H[k - j];
                s += (j & 1) ? -t : t;
            }
            h[k] = s;
        }
        for (k = 0; k <= 6; k++) {
            long long s = h[k];
            for (j = 0; j < k; j++) {
                long long t = av[j] * binom_ll(12 - 2 * j, k - j);
                s -= ((k - j) & 1) ? -t : t;
            }
            av[k] = s;
        }
        done = true;
    }
    if (n > 6) return (seq_t)0;
    return from_i128((__int128)av[n]);
}

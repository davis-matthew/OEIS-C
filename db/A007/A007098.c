/*
 * A007098  Number of unlabeled bisectable trees with 2n+1 nodes.
 * https://oeis.org/A007098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=1..n} binomial(k+1,2) * A228601(n,k), i.e. the sum
 *            of binomial(k+1,2) over all unlabeled trees with n+1 nodes,
 *            where k is the number of vertex orbits of the tree's
 *            automorphism group. It is computed from rooted trees: p(m) =
 *            A000081(m), s1(m) = Sum k and e(m) = Sum k(k-1)/2 over roo...
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 43, u128 n <= 83,
 *            big any n
 * Helpers:   alloc_zero, grow_zero, release, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

static seq_t *P = NULL, *S1 = NULL, *E = NULL, *Q = NULL;
static idx_t cap = 0;

static seq_t half_tri(seq_t x)
{
    if (x % 2 == 0)
        return (x / 2) * (x - 1);
    return x * ((x - 1) / 2);
}

static void compute_p(seq_t *out, idx_t len)
{
    idx_t m, k, j;
    if (seq_is_fixed_width()) {
        unsigned __int128 *pu = (unsigned __int128 *)calloc(len + 1, sizeof(unsigned __int128));
        unsigned __int128 *cu = (unsigned __int128 *)calloc(len + 1, sizeof(unsigned __int128));
        if (len > 1) pu[1] = 1;
        for (m = 1; m < len; m++) {
            for (j = m; j < len; j += m)
                cu[j] += (unsigned __int128)m * pu[m];
            if (m + 1 < len) {
                unsigned __int128 s = 0;
                for (k = 1; k <= m; k++)
                    s += cu[k] * pu[m - k + 1];
                pu[m + 1] = s / m;
            }
        }
        for (m = 0; m < len; m++)
            out[m] = u128_to_seq(pu[m]);
        free(pu);
        free(cu);
    } else {
        seq_t *c = alloc_zero(len + 1);
        for (m = 0; m < len; m++)
            out[m] = 0;
        if (len > 1) out[1] = 1;
        for (m = 1; m < len; m++) {
            for (j = m; j < len; j += m)
                c[j] += out[m] * m;
            if (m + 1 < len) {
                seq_t s = 0;
                for (k = 1; k <= m; k++)
                    s += c[k] * out[m - k + 1];
                out[m + 1] = s / m;
            }
        }
        release(c, len + 1);
    }
}

static void ensure(idx_t nmax)
{
    idx_t newcap, N, i;
    if (nmax < cap)
        return;
    newcap = nmax + 1;
    if (newcap < 2 * cap)
        newcap = 2 * cap;
    if (newcap < 8)
        newcap = 8;
    if (cap == 0) {
        P = alloc_zero(newcap);
        S1 = alloc_zero(newcap);
        E = alloc_zero(newcap);
        Q = alloc_zero(newcap);
    } else {
        P = grow_zero(P, cap, newcap);
        S1 = grow_zero(S1, cap, newcap);
        E = grow_zero(E, cap, newcap);
        Q = grow_zero(Q, cap, newcap);
    }
    compute_p(P, newcap);
    for (N = (cap == 0 ? 1 : cap); N < newcap; N++) {
        seq_t s = P[N], t = 0, q;
        for (i = 1; i < N; i++) {
            s += P[i] * S1[N - i];
            t += P[i] * Q[N - i];
        }
        S1[N] = s;
        E[N] = t;
        q = E[N] + S1[N];
        for (i = 1; 2 * i < N; i++)
            q += S1[i] * S1[N - i];
        if (N % 2 == 0) {
            q -= E[N / 2];
            q += half_tri(S1[N / 2]);
        }
        Q[N] = q;
    }
    cap = newcap;
}

seq_t a(idx_t n)
{
    idx_t N = n + 1, M, b;
    seq_t res;
    ensure(N);
    M = (N - 1) / 2;
    res = E[N] + S1[N];
    for (b = M + 1; b < N; b++) {
        seq_t Fb = E[b] + S1[b];
        seq_t Fo = E[N - b] + S1[N - b];
        res -= Fo * P[b] + P[N - b] * Fb + S1[N - b] * S1[b];
    }
    if (N % 2 == 0) {
        idx_t h = N / 2;
        res += P[h] * (E[h] + S1[h]) + half_tri(S1[h]);
        res -= E[h];
    }
    return res;
}

/*
 * A007835  Number of unordered sets of pairs (in-degree, out-degree) for
 *          nodes of directed trees on n unlabeled nodes (the edges are
 *          directed in arbitrary directions, the tree is unrooted).
 * https://oeis.org/A007835
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = number of multisets of n pairs (i,o) != (0,0) with Sum i
 *            = Sum o = n-1, i.e. vector partitions of (n-1,n-1) into exactly
 *            n parts (every such multiset is realized by a directed tree:
 *            attach a leaf and induct). With V(m,P) = number of vector
 *            partitions of (P,m-P), p(j,<=K) = partitions of j...
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 13, u64 n <= 13, u128 n <= 13,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *a007835_ans = NULL;
static long long a007835_max = 0; /* a(n) cached for 3 <= n <= a007835_max */

static void a007835_build(long long Nn)
{
    long long N = Nn - 2, W = N + 1;
    long long d, p, r, m, P, K, n, m1, m2;
    seq_t *V = alloc_zero((idx_t)(W * W));
    seq_t *pk = alloc_zero((idx_t)((N + 2) * W));
    seq_t *ans = alloc_zero((idx_t)(Nn + 1));

    /* V[m*W+P]: multisets of pairs (p,r) != (0,0) with Sum(p+r)=m, Sum p=P */
    V[0] = 1;
    for (d = 1; d <= N; d++) {
        for (p = 0; p <= d; p++) {
            r = d - p;
            for (m = d; m <= N; m++) {
                seq_t *dst = V + m * W;
                const seq_t *src = V + (m - d) * W;
                for (P = p; P <= m - r; P++)
                    dst[P] += src[P - p];
            }
        }
    }

    /* pk[K*W+m]: partitions of m into parts <= K (= at most K parts) */
    pk[0] = 1;
    for (K = 1; K <= N + 1; K++) {
        for (m = 0; m <= N; m++) {
            pk[K * W + m] = pk[(K - 1) * W + m];
            if (m >= K) pk[K * W + m] += pk[K * W + m - K];
        }
    }

    ans[1] = 1;
    if (Nn >= 2) ans[2] = 1;
    for (n = 3; n <= Nn; n++) {
        seq_t S = 0, T = 0;
        long long tot = n - 2;
        for (m2 = 0; m2 <= tot; m2++) {
            m1 = tot - m2;
            for (P = 0; P <= m2; P++) {
                seq_t v = V[m2 * W + P];
                if (v == 0) continue;
                S += v * pk[(P + 1) * W + m1];
                T += v * pk[N * W + m1];
            }
        }
        ans[n] = S + S - T;
    }

    release(V, (idx_t)(W * W));
    release(pk, (idx_t)((N + 2) * W));
    if (a007835_ans) release(a007835_ans, (idx_t)(a007835_max + 1));
    a007835_ans = ans;
    a007835_max = Nn;
}

seq_t a(idx_t n)
{
    if (n <= 2) return (seq_t)1;
    if ((long long)n > a007835_max) {
        long long t = a007835_max + a007835_max / 2;
        if (t < (long long)n) t = (long long)n;
        if (t < 3) t = 3;
        a007835_build(t);
    }
    return a007835_ans[n];
}

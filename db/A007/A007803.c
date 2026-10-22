/*
 * A007803  Number of connected series-parallel graphs with a longest path of
 *          at most n edges and also a largest cut set of at most n edges.
 * https://oeis.org/A007803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*A391487(n) - 1 = 1 + 2*Sum_{m=2..n} s(m,n), where
 *            s(m,w) is the number of series networks with longest path
 *            exactly m and largest cut at most w. s is computed by duality:
 *            parallel networks are series networks with path and cut swapped.
 *            Let t(l,w) = [l=1] + Sum_{v=2..w} (s(v,l) - s(v,l-1)...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   a7803_gcd, alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/a7803_gcd.h"

static seq_t *a7803_cache = 0;
static idx_t a7803_cache_n = 0;

static void a7803_build(idx_t n)
{
    idx_t N = n + 1;
    idx_t sig, W, w, j, k;
    seq_t *s = alloc_zero(N * N);
    seq_t *F = alloc_zero(N * N);
    seq_t *C = alloc_zero(N + 1);
    seq_t *res = alloc_zero(N);

    for (W = 1; W <= n; W++) F[W * N + 0] = 1;

    for (sig = 2; sig <= 2 * n; sig++) {
        idx_t lo = (sig > n) ? sig - n : 1;
        idx_t hi = (sig - 1 < n) ? sig - 1 : n;
        /* series networks: path exactly m, cut <= W, parts of path < m */
        for (W = lo; W <= hi; W++) {
            idx_t m = sig - W;
            if (m >= 2) s[m * N + W] = F[W * N + m];
        }
        /* non-series networks with path exactly l, cut <= W; add them as parts */
        for (W = lo; W <= hi; W++) {
            idx_t l = sig - W;
            idx_t jmax = n / l;
            seq_t tv = (l == 1) ? 1 : 0;
            seq_t *row = F + W * N;
            for (w = 2; w <= W; w++) {
                tv += s[w * N + l];
                if (l > 1) tv -= s[w * N + l - 1];
            }
            C[0] = 1;
            for (j = 1; j <= jmax; j++) {
                seq_t c = C[j - 1];
                idx_t g = a7803_gcd((idx_t)(c % j), j);
                C[j] = (c / g) * ((tv + (j - 1)) / (j / g));
            }
            for (k = n; k >= l; k--) {
                for (j = 1; j * l <= k; j++)
                    row[k] += C[j] * row[k - j * l];
            }
        }
    }

    for (k = 1; k <= n; k++) {
        seq_t sum = 0;
        for (w = 2; w <= k; w++) sum += s[w * N + k];
        res[k] = 1 + 2 * sum;
    }

    release(s, N * N);
    release(F, N * N);
    release(C, N + 1);
    if (a7803_cache) release(a7803_cache, a7803_cache_n + 1);
    a7803_cache = res;
    a7803_cache_n = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (!a7803_cache || n > a7803_cache_n) a7803_build(n);
    return a7803_cache[n];
}

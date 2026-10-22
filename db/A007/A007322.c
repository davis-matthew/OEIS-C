/*
 * A007322  Number of functors of degree n from free Abelian groups to Abelian
 *          groups.
 * https://oeis.org/A007322
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (1/n!)*Sum_{k=1..n} |Stirling1(n+1,k+1)|*A000670(k)^2
 *            (arbitrary precision); fixed-width builds use the binomial
 *            transform of A101370 (0-1 matrices with m ones and no zero
 *            rows/columns), via a DP
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   alloc_zero, binom, release, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/binom.h"
#include "../utils/seq_is_big.h"

static seq_t *bvals = NULL;
static idx_t bmax = 0;

static void build_b(idx_t N)
{
    idx_t W = N + 1;
    idx_t i, k, m, r, o, q;
    seq_t *C = alloc_zero(W * W);
    seq_t *D = alloc_zero(W * W);
    seq_t *E = alloc_zero(W * W);
    seq_t *nb = alloc_zero(W);

    for (i = 0; i <= N; i++) {
        C[i * W] = 1;
        for (k = 1; k <= i; k++) {
            C[i * W + k] = C[(i - 1) * W + k - 1];
            if (k < i) C[i * W + k] += C[(i - 1) * W + k];
        }
    }

    D[0] = 1;
    for (m = 1; m <= N; m++) {
        for (r = 1; r <= m; r++) {
            seq_t s = 0;
            for (o = 1; o <= r && o + r <= m; o++)
                s += D[r * W + (m - o)] * C[r * W + o];
            E[r * W + m] = s;
        }
        for (r = 1; r <= m; r++) {
            seq_t s = 0;
            for (q = 0; q <= r; q++) {
                idx_t rr = r - q, mm = m - q;
                if (rr > mm) continue;
                s += E[rr * W + mm] * C[r * W + q];
                if (q >= 1)
                    s += D[rr * W + mm] * C[r * W + q];
            }
            D[r * W + m] = s;
        }
        {
            seq_t t = 0;
            for (r = 1; r <= m; r++) t += D[r * W + m];
            nb[m] = t;
        }
    }

    release(C, W * W);
    release(D, W * W);
    release(E, W * W);
    if (bvals) release(bvals, bmax + 1);
    bvals = nb;
    bmax = N;
}

static seq_t big_a(idx_t n)
{
    idx_t m, k, j;
    seq_t *c = alloc_zero(n + 2);
    seq_t *T = alloc_zero(n + 2);
    seq_t sum = 0, res;

    /* unsigned Stirling numbers of the first kind, row n+1 */
    c[0] = 1;
    for (m = 1; m <= n + 1; m++) {
        for (k = m; k >= 1; k--)
            c[k] = c[k - 1] + c[k] * (seq_t)(m - 1);
        c[0] = 0;
    }

    /* T(k,j) = j!*S2(k,j); A000670(k) = Sum_j T(k,j) */
    T[0] = 1;
    for (k = 1; k <= n; k++) {
        seq_t F = 0;
        for (j = k; j >= 1; j--)
            T[j] = (T[j] + T[j - 1]) * (seq_t)j;
        T[0] = 0;
        for (j = 1; j <= k; j++) F += T[j];
        sum += c[k + 1] * F * F;
    }

    res = sum / c[1];   /* c[1] = n! */
    release(c, n + 2);
    release(T, n + 2);
    return res;
}

seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t m;
    if (n < 1) return 0;
    if (seq_is_big()) return big_a(n);
    if (bvals == NULL || n > bmax) build_b(n);
    for (m = 1; m <= n; m++)
        sum += binom(n, m) * bvals[m];
    return sum;
}

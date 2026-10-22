/*
 * A003495  High temperature series for susceptibility for spherical model on
 *          f.c.c. lattice.
 * https://oeis.org/A003495
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, more
 * Formula:   the spherical model gives chi = 1/((1-12t)P(t)) with K = t*P(t),
 *            where P(t) = Sum_{m>=0} c(m) t^m and c(m) (A002899) counts
 *            closed m-step walks on the f.c.c. lattice: c(m) = Sum_{i+j+k=m}
 *            m!/(i!j!k!) * cb(i+j)*cb(j+k)*cb(k+i), cb(2r) = C(2r,r), cb(odd)
 *            = 0. So chi = t/(K(1-12t)), i.e. a(n) = Sum_...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *a3495_cache = NULL;
static idx_t a3495_have = 0; /* number of cached terms */

static void a3495_compute(idx_t n)
{
    idx_t N = n + 1, W = N + 1, p, j, i, m, q;
    seq_t *B = (seq_t *)calloc((size_t)(W * W), sizeof(seq_t));
    seq_t *c = (seq_t *)calloc((size_t)W, sizeof(seq_t));
    seq_t *TP = (seq_t *)calloc((size_t)(W * W), sizeof(seq_t));
    seq_t *TN = (seq_t *)calloc((size_t)(W * W), sizeof(seq_t));
    seq_t *res = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));

    /* Pascal triangle (additions only, exact modulo 2^w) */
    for (i = 0; i <= N; i++) {
        B[i * W + 0] = (seq_t)1;
        for (j = 1; j <= i; j++) {
            B[i * W + j] = B[(i - 1) * W + j - 1];
            if (j <= i - 1) B[i * W + j] += B[(i - 1) * W + j];
        }
    }

    /* closed walks on fcc */
    for (m = 0; m < N; m++) {
        seq_t s = (seq_t)0;
        for (i = 0; i <= m; i++) {
            for (j = 0; i + j <= m; j++) {
                idx_t k = m - i - j;
                idx_t u = i + k, v = i + j, w = j + k;
                if ((u & 1) || (v & 1) || (w & 1)) continue;
                {
                    seq_t t = B[m * W + i];
                    t = t * B[(m - i) * W + j];
                    t = t * B[u * W + u / 2];
                    t = t * B[v * W + v / 2];
                    t = t * B[w * W + w / 2];
                    s += t;
                }
            }
        }
        c[m] = s;
    }

    /* T[p][j] = [K^j] t^p, stored as positive part TP minus negative part TN */
    TP[1 * W + 1] = (seq_t)1;
    for (j = 2; j <= N; j++) {
        seq_t sp = (seq_t)0, sn = (seq_t)0;
        for (p = 2; p <= j; p++) {
            seq_t xp = (seq_t)0, xn = (seq_t)0;
            for (i = 1; i + p - 1 <= j; i++) {
                seq_t ap = TP[1 * W + i], an = TN[1 * W + i];
                seq_t bp = TP[(p - 1) * W + (j - i)], bn = TN[(p - 1) * W + (j - i)];
                xp += ap * bp + an * bn;
                xn += ap * bn + an * bp;
            }
            TP[p * W + j] = xp;
            TN[p * W + j] = xn;
            sp += c[p - 1] * xp;
            sn += c[p - 1] * xn;
        }
        /* t^1 coefficient = -Sum c(p-1) T[p][j] */
        TP[1 * W + j] = sn;
        TN[1 * W + j] = sp;
    }

    for (q = 0; q <= n; q++) {
        idx_t J = q + 1;
        seq_t pw = (seq_t)1, sp = (seq_t)0, sn = (seq_t)0;
        for (p = 1; p <= J; p++) {
            sp += pw * TP[p * W + J];
            sn += pw * TN[p * W + J];
            pw *= 12;
        }
        res[q] = sp - sn;
    }

    if (a3495_cache) free(a3495_cache);
    a3495_cache = res;
    a3495_have = n + 1;

    free(B);
    free(c);
    free(TP);
    free(TN);
}

seq_t a(idx_t n)
{
    if (n >= a3495_have) {
        idx_t target = n;
        if (a3495_have > 0 && target < 2 * a3495_have) target = 2 * a3495_have;
        if (target < 32) target = 32;
        if (target < n) target = n;
        a3495_compute(target);
    }
    return a3495_cache[n];
}

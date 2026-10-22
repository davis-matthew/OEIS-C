/*
 * A005146  Numerators of numbers occurring in continued fraction connected
 *          with expansion of gamma function.
 * https://oeis.org/A005146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, nice
 * Formula:   a(n) = numerator of the n-th coefficient of the Stieltjes
 *            continued fraction sum_{k>=0} c_k x^k = a_0/(1 - a_1 x/(1 - a_2
 *            x/(1 - ...))), c_k = |B_{2k+2}|/((2k+1)(2k+2)) =
 *            T(k+1)/(4^(k+1)*(4^(k+1)-1)*(2k+1)), T = tangent numbers
 *            (A000182); a_0 = c_0 = 1/12, a_1 = 1/30, a_2 = 53/210, ... The
 *            a_n (n...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

#ifdef __cplusplus
typedef seq_t num_t;
static num_t *nalloc(idx_t len) { return alloc_zero(len); }
static void nfree(num_t *v, idx_t len) { release(v, len); }
#else
typedef unsigned __int128 num_t;
static num_t *nalloc(idx_t len) { return (num_t *)calloc((size_t)len, sizeof(num_t)); }
static void nfree(num_t *v, idx_t len) { (void)len; free(v); }
#endif

static num_t ngcd(num_t a, num_t b)
{
    while (b != 0) {
        num_t t = a % b;
        a = b;
        b = t;
    }
    return a;
}

static void rmul(num_t ap, num_t aq, num_t bp, num_t bq, num_t *rp, num_t *rq)
{
    if (ap == 0 || bp == 0) {
        *rp = 0;
        *rq = 1;
        return;
    }
    num_t g1 = ngcd(ap, bq), g2 = ngcd(bp, aq);
    num_t p = (ap / g1) * (bp / g2);
    num_t q = (aq / g2) * (bq / g1);
    *rp = p;
    *rq = q;
}

static void radd(num_t ap, num_t aq, num_t bp, num_t bq, num_t *rp, num_t *rq)
{
    num_t g = ngcd(aq, bq);
    num_t t = ap * (bq / g) + bp * (aq / g);
    if (t == 0) {
        *rp = 0;
        *rq = 1;
        return;
    }
    num_t g2 = ngcd(t, g);
    num_t q = (aq / g) * (bq / g2);
    *rp = t / g2;
    *rq = q;
}

/* a - b, assumes a >= b */
static void rsub(num_t ap, num_t aq, num_t bp, num_t bq, num_t *rp, num_t *rq)
{
    num_t g = ngcd(aq, bq);
    num_t t = ap * (bq / g) - bp * (aq / g);
    if (t == 0) {
        *rp = 0;
        *rq = 1;
        return;
    }
    num_t g2 = ngcd(t, g);
    num_t q = (aq / g) * (bq / g2);
    *rp = t / g2;
    *rq = q;
}

static num_t *cache = NULL;
static idx_t cacheN = 0;

/* returns array res[0..N] of numerators of a_0..a_N */
static num_t *compute(idx_t N)
{
    idx_t M = N + 1, k, j, i, L;
    num_t *res = nalloc(N + 2);
    num_t *T = nalloc(M + 2);
    num_t *cp = nalloc(N + 2), *cq = nalloc(N + 2);
    num_t *Qp = nalloc(N + 2), *Qq = nalloc(N + 2);
    num_t *Ep = nalloc(N + 2), *Eq = nalloc(N + 2);
    num_t pw;

    /* tangent numbers T[1..M] */
    T[1] = 1;
    for (k = 2; k <= M; k++)
        T[k] = T[k - 1] * (k - 1);
    for (k = 2; k <= M; k++)
        for (j = k; j <= M; j++)
            T[j] = T[j - 1] * (j - k) + T[j] * (j - k + 2);

    /* moments c_0..c_N */
    pw = 1;
    for (k = 0; k <= N; k++) {
        num_t den, g;
        pw = pw * 4;
        den = pw * (pw - 1);
        den = den * (2 * k + 1);
        g = ngcd(T[k + 1], den);
        cp[k] = T[k + 1] / g;
        cq[k] = den / g;
    }
    res[0] = cp[0];

    for (k = 0; k < N; k++) {
        rmul(cp[k + 1], cq[k + 1], cq[k], cp[k], &Qp[k], &Qq[k]);
        Ep[k] = 0;
        Eq[k] = 1;
    }
    Ep[N] = 0;
    Eq[N] = 1;

    if (N >= 1) {
        res[1] = Qp[0];
        L = N;
        i = 1;
        while (i < N) {
            /* e step */
            for (k = 0; k + 1 < L; k++) {
                num_t sp, sq;
                radd(Qp[k + 1], Qq[k + 1], Ep[k + 1], Eq[k + 1], &sp, &sq);
                rsub(sp, sq, Qp[k], Qq[k], &Ep[k], &Eq[k]);
            }
            L--;
            i++;
            res[i] = Ep[0];
            if (i >= N)
                break;
            /* q step */
            for (k = 0; k + 1 < L; k++) {
                num_t sp, sq;
                rmul(Qp[k + 1], Qq[k + 1], Ep[k + 1], Eq[k + 1], &sp, &sq);
                rmul(sp, sq, Eq[k], Ep[k], &Qp[k], &Qq[k]);
            }
            L--;
            i++;
            res[i] = Qp[0];
        }
    }

    nfree(T, M + 2);
    nfree(cp, N + 2);
    nfree(cq, N + 2);
    nfree(Qp, N + 2);
    nfree(Qq, N + 2);
    nfree(Ep, N + 2);
    nfree(Eq, N + 2);
    return res;
}

seq_t a(idx_t n)
{
    if (cache == NULL || n > cacheN) {
        idx_t N = n;
        if (cache != NULL && N < cacheN + cacheN / 2)
            N = cacheN + cacheN / 2;
        num_t *r = compute(N);
        if (cache != NULL)
            nfree(cache, cacheN + 2);
        cache = r;
        cacheN = N;
    }
    return (seq_t)cache[n];
}

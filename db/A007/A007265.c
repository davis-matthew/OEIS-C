/*
 * A007265  McKay-Thompson series of class 8A for Monster.
 * https://oeis.org/A007265
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   T8A = t + 4 + 32/t, where t =
 *            eta(q)^4*eta(q^4)^2/(eta(q^2)^2*eta(q^8)^4) =
 *            phi(-q)^2/(q*psi(q^4)^2) is the Hauptmodul for Gamma0(8). With P
 *            = q*t = phi(-q)^2/psi(q^4)^2 and Q = 1/(q*t) =
 *            psi(q^4)^2/phi(-q)^2: a(-1) = 1, a(0) = 0, a(n) = [q^(n+1)] P +
 *            32*[q^(n-1)] Q for n >= 1
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 33, u64 n <= 120, u128 n <= 449,
 *            big any n
 * Helpers:   grow_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/grow_zero.h"

static seq_t *Qs = NULL;          /* coefficients of Q (all >= 0) */
static seq_t *Pm = NULL;          /* magnitudes of coefficients of P */
static unsigned char *Pn = NULL;  /* sign flags of coefficients of P */
static idx_t L = 0;

static void extend(idx_t M)
{
    idx_t cap = L + L / 2;
    if (cap < M) cap = M;
    if (cap < 64) cap = 64;

    idx_t *r2 = (idx_t *)calloc((size_t)cap, sizeof(idx_t));
    idx_t *g = (idx_t *)calloc((size_t)cap, sizeof(idx_t));
    idx_t *rk = (idx_t *)malloc((size_t)cap * sizeof(idx_t));
    idx_t *gk = (idx_t *)malloc((size_t)cap * sizeof(idx_t));
    idx_t nr = 0, ng = 0, x, y, i, j, k, m;

    /* r2(k): representations as x^2 + y^2 over Z^2; phi(-q)^2 = sum (-1)^k r2(k) q^k */
    for (x = 0; x * x < cap; x++)
        for (y = 0; x * x + y * y < cap; y++) {
            idx_t w = (x ? 2 : 1) * (y ? 2 : 1);
            r2[x * x + y * y] += w;
        }
    /* psi(q^4)^2 = sum_{i,j>=0} q^(4*(T_i+T_j)) */
    for (i = 0; 4 * (i * (i + 1) / 2) < cap; i++)
        for (j = 0; 4 * (i * (i + 1) / 2 + j * (j + 1) / 2) < cap; j++)
            g[4 * (i * (i + 1) / 2 + j * (j + 1) / 2)]++;

    for (k = 1; k < cap; k++) {
        if (r2[k]) rk[nr++] = k;
        if (g[k]) gk[ng++] = k;
    }

    Qs = grow_zero(Qs, L, cap);
    Pm = grow_zero(Pm, L, cap);
    Pn = (unsigned char *)realloc(Pn, (size_t)cap);

    for (m = L; m < cap; m++) {
        /* Q * phi(-q)^2 = psi(q^4)^2 */
        seq_t pos = 0, neg = 0;
        pos += g[m];
        for (i = 0; i < nr && rk[i] <= m; i++) {
            k = rk[i];
            if (k & 1) pos += Qs[m - k] * r2[k];
            else neg += Qs[m - k] * r2[k];
        }
        Qs[m] = pos - neg;

        /* P * psi(q^4)^2 = phi(-q)^2 */
        seq_t pp = 0, pq = 0;
        if (m & 1) pq += r2[m];
        else pp += r2[m];
        for (i = 0; i < ng && gk[i] <= m; i++) {
            k = gk[i];
            if (Pn[m - k]) pp += Pm[m - k] * g[k];
            else pq += Pm[m - k] * g[k];
        }
        if (pp >= pq) { Pm[m] = pp - pq; Pn[m] = 0; }
        else { Pm[m] = pq - pp; Pn[m] = 1; }
    }

    L = cap;
    free(r2); free(g); free(rk); free(gk);
}

seq_t a(idx_t n)
{
    if (n == -1) return 1;
    if (n == 0) return 0;
    if (n + 2 > L) extend(n + 2);
    seq_t res = Qs[n - 1] * 32;
    if (Pn[n + 1]) res -= Pm[n + 1];
    else res += Pm[n + 1];
    return res;
}

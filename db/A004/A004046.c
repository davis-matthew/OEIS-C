/*
 * A004046  Theta series of extremal 3-modular even 24-dimensional lattice
 *          with minimal norm 6 and det = 3^12.
 * https://oeis.org/A004046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. = (27*a(q)^12 - 72*a(q)^9*b(q)^3 + 64*a(q)^6*b(q)^6 +
 *            16*a(q)^3*b(q)^9 - 8*b(q)^12)/27 with cubic AGM theta functions.
 *            Using b^3 = a^3 - c^3 and c(q)^3 = 27*C(q), and writing A = a^3:
 *            G.f. = A^4 - 72*A^3*C + 1728*A^2*C^2 + 11664*A*C^3 - 157464*C^4,
 *            where a(q) = 1 + 6*Sum_{n>=1} E(n) q^n, C(q...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 9, u64 n <= 67, u128 n <= 2000,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *A2s = 0, *C2s = 0, *ACs = 0;
static idx_t cap = 0; /* arrays hold cap coefficients */

static seq_t *smul(const seq_t *x, const seq_t *y, idx_t len)
{
    seq_t *out = alloc_zero(len);
    idx_t k, i;
    for (k = 0; k < len; k++) {
        seq_t acc = 0;
        for (i = 0; i <= k; i++)
            acc += x[i] * y[k - i];
        out[k] = acc;
    }
    return out;
}

static void build(idx_t len)
{
    idx_t M = 3 * len + 2, d, m, k;
    int *E = (int *)calloc((size_t)(M + 1), sizeof(int));
    seq_t *av, *sv, *a2, *A, *s2, *s3, *Cp;
    for (d = 1; d <= M; d++) {
        int chi = (d % 3 == 1) ? 1 : ((d % 3 == 2) ? -1 : 0);
        if (!chi) continue;
        for (m = d; m <= M; m += d) E[m] += chi;
    }
    av = alloc_zero(len);
    sv = alloc_zero(len);
    av[0] = 1;
    for (k = 1; k < len; k++) av[k] = (seq_t)(6 * E[k]);
    for (k = 0; k < len; k++) sv[k] = (seq_t)E[3 * k + 1];
    free(E);

    a2 = smul(av, av, len);
    A = smul(a2, av, len);
    s2 = smul(sv, sv, len);
    s3 = smul(s2, sv, len);
    Cp = alloc_zero(len);
    for (k = 1; k < len; k++) Cp[k] = s3[k - 1];

    if (cap) {
        release(A2s, cap);
        release(C2s, cap);
        release(ACs, cap);
    }
    A2s = smul(A, A, len);
    C2s = smul(Cp, Cp, len);
    ACs = smul(A, Cp, len);
    cap = len;

    release(av, len);
    release(sv, len);
    release(a2, len);
    release(A, len);
    release(s2, len);
    release(s3, len);
    release(Cp, len);
}

seq_t a(idx_t n)
{
    seq_t p1 = 0, p2 = 0, p3 = 0, q1 = 0, q2 = 0;
    idx_t i;
    if (n + 1 > cap) {
        idx_t len = 2 * cap;
        if (len < n + 1) len = n + 1;
        if (len < 64) len = 64;
        build(len);
    }
    for (i = 0; i <= n; i++) {
        p1 += A2s[i] * A2s[n - i];
        p2 += A2s[i] * C2s[n - i];
        p3 += ACs[i] * C2s[n - i];
        q1 += A2s[i] * ACs[n - i];
        q2 += C2s[i] * C2s[n - i];
    }
    return (p1 + p2 * 1728 + p3 * 11664) - (q1 * 72 + q2 * 157464);
}

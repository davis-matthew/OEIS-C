/*
 * A004490  Colossally abundant numbers: m for which there is a positive
 *          exponent epsilon such that sigma(m)/m^{1 + epsilon} >=
 *          sigma(k)/k^{1 + epsilon} for all k > 1, so that m attains the
 *          maximum value of sigma(m)/m^{1 + epsilon}.
 * https://oeis.org/A004490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Product_{k=1..n} A073751(k). Each step multiplies by the
 *            prime p whose next critical exponent eps(p,k) = log(1 + 1/(p +
 *            p^2 + ... + p^k)) / log(p) is the largest one not used yet (a
 *            greedy merge of the critical epsilons)
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 14, u64 n <= 23, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <math.h>

static uint32_t *ca_chosen = 0;
static idx_t ca_len = 0;

static void ca_build(idx_t N)
{
    idx_t m = N + 20, lim, i, j, np = 0, na = 0, step;
    double lm;
    char *sv;
    uint32_t *P;
    long double *s, *e;
    if (m < 6) m = 6;
    lm = log((double)m);
    lim = (idx_t)((double)m * (lm + log(lm))) + 20;
    sv = (char *)calloc((size_t)lim + 1, 1);
    P = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)m + 1));
    for (i = 2; i <= lim && np < m; i++) {
        if (sv[i]) continue;
        P[np++] = (uint32_t)i;
        for (j = i * i; j <= lim; j += i) sv[j] = 1;
    }
    free(sv);
    s = (long double *)malloc(sizeof(long double) * ((size_t)np + 1));
    e = (long double *)malloc(sizeof(long double) * ((size_t)np + 1));
    free(ca_chosen);
    ca_chosen = (uint32_t *)malloc(sizeof(uint32_t) * ((size_t)N + 1));
    for (step = 0; step < N; step++) {
        long double p, best;
        idx_t bi = na;
        p = (long double)P[na];
        best = log1pl(1.0L / p) / logl(p);
        for (i = 0; i < na; i++)
            if (e[i] > best) { best = e[i]; bi = i; }
        if (bi == na) {
            s[na] = p;
            na++;
        }
        p = (long double)P[bi];
        s[bi] = p * (s[bi] + 1.0L);
        e[bi] = log1pl(1.0L / s[bi]) / logl(p);
        ca_chosen[step] = P[bi];
    }
    free(s);
    free(e);
    free(P);
    ca_len = N;
}

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t i;
    if (n > ca_len) {
        idx_t want = n;
        if (want < 2 * ca_len) want = 2 * ca_len;
        if (want < 64) want = 64;
        ca_build(want);
    }
    for (i = 0; i < n; i++) r *= (seq_t)ca_chosen[i];
    return r;
}

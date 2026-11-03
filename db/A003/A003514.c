/*
 * A003514  Number of series-reduced labeled graphs with n nodes.
 * https://oeis.org/A003514
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=0..n} (-1)^(n-k) * binomial(n,k) * 2^binomial(k,2)
 *            * M(n-k,k), by inclusion-exclusion over a set S of s = n-k
 *            vertices that are forced to have degree 2. M(s,k) is s! [x^s] of
 *            (1-x)^(-1/2)*exp(-x/2-x^2/4)*exp(k*x^2/(2(1-x)))*exp(binomial(k,2)*x/(1-x)),
 *            which is the same as the e.g.f....
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Validated: standard, pass, 33 terms through n = 76 (2026-11-03)
 * Helpers:   mem, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t N = n, j, k, m;
    seq_t *fact = mem_new(seq_t, N + 2);
    seq_t *hf = mem_new(seq_t, N + 2);   /* j!/2 for j >= 2 */
    seq_t *cn = mem_new(seq_t, N + 2);   /* binomial(n, .) */
    seq_t *row = mem_new(seq_t, N + 2);
    seq_t *aa = mem_new(seq_t, N + 2);
    seq_t *b = mem_new(seq_t, N + 2);
    seq_t pos = 0, neg = 0, res;

    fact[0] = 1;
    for (j = 1; j <= N; j++) fact[j] = fact[j - 1] * (seq_t)j;
    hf[0] = 0; hf[1] = 0;
    if (N >= 2) hf[2] = 1;
    for (j = 3; j <= N; j++) hf[j] = hf[j - 1] * (seq_t)j;

    /* binomial row n via Pascal */
    for (j = 0; j <= N; j++) cn[j] = 0;
    cn[0] = 1;
    for (m = 1; m <= N; m++) {
        for (j = m; j >= 1; j--) cn[j] += cn[j - 1];
    }

    for (k = 0; k <= N; k++) {
        idx_t s = N - k;
        idx_t ck = k * (k - (k ? 1 : 0)) / 2;
        seq_t c = (seq_t)ck;
        seq_t term;

        for (j = 1; j <= s; j++) {
            seq_t v = c * fact[j];
            if (j >= 2) v += (seq_t)k * hf[j];
            if (j >= 3) v += hf[j - 1];
            aa[j] = v;
        }
        for (j = 0; j <= s; j++) row[j] = 0;
        row[0] = 1;              /* row = binomial(m-1, .) */
        b[0] = 1;
        for (m = 1; m <= s; m++) {
            seq_t acc = 0;
            for (j = 1; j <= m; j++)
                acc += row[j - 1] * aa[j] * b[m - j];
            b[m] = acc;
            /* advance row to binomial(m, .) */
            for (j = m; j >= 1; j--) row[j] += row[j - 1];
        }
        term = cn[k] * pow2(ck) * b[s];
        if (s & 1) neg += term; else pos += term;
    }
    res = pos - neg;

    mem_free(fact); mem_free(hf); mem_free(cn); mem_free(row); mem_free(aa); mem_free(b);
    return res;
}

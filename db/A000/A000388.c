/*
 * A000388  Number of permutations of an n-sequence discordant with three
 *          given permutations (see reference) in n-2 places.
 * https://oeis.org/A000388
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} (-1)^k * binomial(k,2) * sigma_{n,k} *
 *            (n-k)!, where sigma_{n,k} = [u^n t^k] sigma(t,u) and sigma(t,u)
 *            = (1 - 2t^2u^2 - 2t^2(1+t)u^3 + 3t^4u^4) /
 *            ((1-tu)(1-(1+2t)u-tu^2+t^3u^3)). The denominator is 1 - (1+3t)u
 *            + 2t^2u^2 + (t^2+t^3)u^3 - t^4u^4, so s_m(t) = N_m(t) +
 *            (1+3t)s_{m-...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    long long N = (long long)n;
    seq_t *buf[5];
    long long b, m, j, k;
    seq_t result, fact, c;

    for (b = 0; b < 5; b++) {
        buf[b] = (seq_t *)malloc((size_t)(N + 1) * sizeof(seq_t));
        for (j = 0; j <= N; j++) buf[b][j] = 0;
    }

    for (m = 0; m <= N; m++) {
        seq_t *cur = buf[m % 5];
        seq_t *p1 = m >= 1 ? buf[(m - 1) % 5] : 0;
        seq_t *p2 = m >= 2 ? buf[(m - 2) % 5] : 0;
        seq_t *p3 = m >= 3 ? buf[(m - 3) % 5] : 0;
        seq_t *p4 = m >= 4 ? buf[(m - 4) % 5] : 0;
        for (j = 0; j <= m && j <= N; j++) {
            seq_t v = 0;
            if (m == 0 && j == 0) v += 1;
            if (m == 2 && j == 2) v -= 2;
            if (m == 3 && (j == 2 || j == 3)) v -= 2;
            if (m == 4 && j == 4) v += 3;
            if (p1) {
                v += p1[j];
                if (j >= 1) v += 3 * p1[j - 1];
            }
            if (p2 && j >= 2) v -= 2 * p2[j - 2];
            if (p3) {
                if (j >= 2) v -= p3[j - 2];
                if (j >= 3) v -= p3[j - 3];
            }
            if (p4 && j >= 4) v += p4[j - 4];
            cur[j] = v;
        }
    }

    {
        seq_t *s = buf[N % 5];
        result = 0;
        fact = 1; /* (N-k)! */
        for (k = N; k >= 2; k--) {
            if (k < N) fact *= (seq_t)(N - k);
            if (k % 2 == 0) c = (seq_t)(k / 2) * (seq_t)(k - 1);
            else c = (seq_t)k * (seq_t)((k - 1) / 2);
            if (k % 2 == 0) result += s[k] * fact * c;
            else result -= s[k] * fact * c;
        }
    }

    for (b = 0; b < 5; b++) free(buf[b]);
    return result;
}

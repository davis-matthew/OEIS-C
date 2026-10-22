/*
 * A000440  Number of permutations of an n-sequence discordant with three
 *          given permutations (see reference) in n-4 places.
 * https://oeis.org/A000440
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=4..n} (-1)^(k-4) * C(k,4) * sigma_{n,k} * (n-k)!,
 *            where Sum_{n,k} sigma_{n,k} t^k u^n =
 *            (1-2t^2u^2-2t^2(1+t)u^3+3t^4u^4) /
 *            ((1-tu)(1-(1+2t)u-tu^2+t^3u^3)), i.e. row polynomials S_n(t) =
 *            Sum_k sigma_{n,k} t^k satisfy S_n = (1+3t)S_{n-1} - 2t^2 S_{n-2}
 *            - (t^2+t^3) S_{n-3} + t^4 S_{n-4...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    idx_t len = n + 1, m, j, k;
    seq_t *buf[5];
    seq_t *S;
    seq_t res = 0, fact = 1;
    int i;

    for (i = 0; i < 5; i++)
        buf[i] = (seq_t *)calloc((size_t)len, sizeof(seq_t));

    for (m = 0; m <= n; m++) {
        seq_t *cur = buf[m % 5];
        seq_t *s1 = buf[(m + 4) % 5];
        seq_t *s2 = buf[(m + 3) % 5];
        seq_t *s3 = buf[(m + 2) % 5];
        seq_t *s4 = buf[(m + 1) % 5];
        for (j = 0; j <= m; j++) {
            seq_t v = 0;
            if (m >= 1) {
                v += s1[j];
                if (j >= 1) v += 3 * s1[j - 1];
            }
            if (m >= 2 && j >= 2) v -= 2 * s2[j - 2];
            if (m >= 3) {
                if (j >= 2) v -= s3[j - 2];
                if (j >= 3) v -= s3[j - 3];
            }
            if (m >= 4 && j >= 4) v += s4[j - 4];
            if (m == 0 && j == 0) v += 1;
            if (m == 2 && j == 2) v -= 2;
            if (m == 3 && (j == 2 || j == 3)) v -= 2;
            if (m == 4 && j == 4) v += 3;
            cur[j] = v;
        }
    }
    S = buf[n % 5];

    for (k = n; k >= 4; k--) {
        unsigned long long kk = (unsigned long long)k;
        unsigned long long b = kk * (kk - 1) / 2 * (kk - 2) / 3 * (kk - 3) / 4;
        seq_t term = S[k] * fact * (seq_t)b;
        if (((k - 4) & 1) == 0) res += term;
        else res -= term;
        fact *= (seq_t)(n - k + 1);
    }

    for (i = 0; i < 5; i++) free(buf[i]);
    return res;
}

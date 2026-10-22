/*
 * A000380  Number of permutations of an n-sequence discordant with three
 *          given permutations (see reference) in n-3 places.
 * https://oeis.org/A000380
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=3..n} (-1)^(k-3) * C(k,3) * (n-k)! * sigma_{n,k},
 *            where sigma_{n,k} = [u^n t^k] (1 - 2t^2u^2 - 2t^2(1+t)u^3 +
 *            3t^4u^4) / ((1-tu)(1-(1+2t)u-tu^2+t^3u^3)). With s_n(t) = Sum_k
 *            sigma_{n,k} t^k this is the recurrence s_n = (1+3t)s_{n-1} -
 *            2t^2 s_{n-2} - (t^2+t^3)s_{n-3} + t^4 s_{n-4} +...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 13, u64 n <= 21, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    idx_t L = n + 1;
    seq_t *R[5];
    int i;
    for (i = 0; i < 5; i++) R[i] = (seq_t *)calloc((size_t)L, sizeof(seq_t));

    for (idx_t m = 0; m <= n; m++) {
        seq_t *cur = R[m % 5];
        seq_t *s1 = m >= 1 ? R[(m - 1) % 5] : 0;
        seq_t *s2 = m >= 2 ? R[(m - 2) % 5] : 0;
        seq_t *s3 = m >= 3 ? R[(m - 3) % 5] : 0;
        seq_t *s4 = m >= 4 ? R[(m - 4) % 5] : 0;
        for (idx_t j = 0; j <= m; j++) {
            seq_t p = 0, q = 0;
            if (m >= 1) {
                p += s1[j];
                if (j >= 1) p += 3 * s1[j - 1];
            }
            if (m >= 2 && j >= 2) q += 2 * s2[j - 2];
            if (m >= 3) {
                if (j >= 2) q += s3[j - 2];
                if (j >= 3) q += s3[j - 3];
            }
            if (m >= 4 && j >= 4) p += s4[j - 4];
            if (m == 0 && j == 0) p += 1;
            if (m == 2 && j == 2) q += 2;
            if (m == 3 && (j == 2 || j == 3)) q += 2;
            if (m == 4 && j == 4) p += 3;
            cur[j] = p - q;
        }
    }

    seq_t *sig = R[n % 5];
    seq_t pos = 0, neg = 0;
    seq_t fact = 1; /* (n-k)! */
    for (idx_t k = n; k >= 3; k--) {
        if (k < n) fact *= (seq_t)(n - k);
        uint64_t kk = (uint64_t)k;
        uint64_t b = kk * (kk - 1) / 2 * (kk - 2) / 3; /* C(k,3) */
        seq_t term = sig[k] * (seq_t)b * fact;
        if ((k - 3) % 2 == 0) pos += term;
        else neg += term;
    }

    for (i = 0; i < 5; i++) free(R[i]);
    return pos - neg;
}

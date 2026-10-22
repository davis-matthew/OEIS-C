/*
 * A002996  a(n) = Sum_{k|n} mu(k)*Catalan(n/k) (mu = Moebius function
 *          A008683).
 * https://oeis.org/A002996
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = Sum_{k|n} mu(k)*Catalan(n/k), Catalan(m) =
 *            binomial(2m,m)/(m+1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   a2996_is_fixed, a2996_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2996_is_fixed.h"
#include "../utils/a2996_mu.h"

seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    idx_t m;
    if (a2996_is_fixed()) {
        /* exact modulo 2^w: Catalan by convolution (no division) */
        seq_t *C = (seq_t *)malloc((size_t)(n + 1) * sizeof(seq_t));
        C[0] = 1;
        for (m = 1; m <= n; m++) {
            seq_t s = 0;
            idx_t i, h = m / 2;
            for (i = 0; i < h; i++) {
                seq_t t = (seq_t)((C[i] + 0u) * C[m - 1 - i]);
                s = (seq_t)(s + t + t);
            }
            if (m % 2 == 1) s = (seq_t)(s + (C[h] + 0u) * C[h]);
            C[m] = s;
        }
        for (m = 1; m <= n; m++) {
            if (n % m == 0) {
                int mu = a2996_mu((uint64_t)(n / m));
                if (mu == 1) pos = (seq_t)(pos + C[m]);
                else if (mu == -1) neg = (seq_t)(neg + C[m]);
            }
        }
        free(C);
        return (seq_t)(pos - neg);
    } else {
        seq_t C = 1;
        for (m = 1; m <= n; m++) {
            C = C * (4 * m - 2) / (m + 1);
            if (n % m == 0) {
                int mu = a2996_mu((uint64_t)(n / m));
                if (mu == 1) pos += C;
                else if (mu == -1) neg += C;
            }
        }
        return pos - neg;
    }
}

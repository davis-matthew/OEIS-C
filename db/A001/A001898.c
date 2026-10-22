/*
 * A001898  Denominators of Bernoulli polynomials B(n)(x).
 * https://oeis.org/A001898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{p prime, p-1 <= n} p^e(p), where e(p) = max(0,
 *            Sum_{i>=0} floor(n/((p-1)*p^i)) - Sum_{i>=1} floor(n/p^i)), i.e.
 *            A091137(n) (exact denominator of B(n)(x)/n!) with the factor n!
 *            removed
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 17, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t result = 1;
    uint64_t lim, i, j, p;
    unsigned char *comp;
    if (n == 0) return 1;
    lim = (uint64_t)n + 1;
    comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    for (i = 2; i <= lim; i++) {
        if (comp[i]) continue;
        if (i <= lim / i)
            for (j = i * i; j <= lim; j += i) comp[j] = 1;
        p = i;
        {
            uint64_t e1 = 0, e2 = 0, q, d;
            int64_t e;
            /* Sum floor(n/((p-1) p^i)) */
            q = 1;
            for (;;) {
                d = (p - 1) * q;
                if (d > n) break;
                e1 += n / d;
                if (q > n / p) break;
                q *= p;
            }
            /* v_p(n!) */
            q = p;
            while (q <= n) {
                e2 += n / q;
                if (q > n / p) break;
                q *= p;
            }
            e = (int64_t)e1 - (int64_t)e2;
            while (e > 0) {
                result *= (seq_t)p;
                e--;
            }
        }
    }
    free(comp);
    return result;
}

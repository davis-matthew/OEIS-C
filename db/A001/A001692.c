/*
 * A001692  Number of irreducible polynomials of degree n over GF(5);
 *          dimensions of free Lie algebras.
 * https://oeis.org/A001692
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{d|n} mu(d)*5^(n/d)/n for n>0, a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   a1692_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a1692_mu.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* coef[k] = mu(n/k) if k | n, else 0 */
    int *coef = (int *)malloc((size_t)(n + 1) * sizeof(int));
    idx_t k;
    for (k = 0; k <= n; k++) coef[k] = 0;
    for (k = 1; k * k <= n; k++) {
        if (n % k == 0) {
            idx_t k2 = n / k;
            coef[k] = a1692_mu(k2);
            coef[k2] = a1692_mu(k);
        }
    }

    /* maintain 5^k = q*n + r with 0 <= r < n */
    seq_t q = 0;
    uint64_t r = 1 % n;
    if (n == 1) q = 1;
    seq_t pos = 0, neg = 0;
    long long rs = 0;
    for (k = 1; k <= n; k++) {
        uint64_t r5 = r * 5;
        q = q * 5 + (seq_t)(r5 / n);
        r = r5 % n;
        if (coef[k] > 0) {
            pos += q;
            rs += (long long)r;
        } else if (coef[k] < 0) {
            neg += q;
            rs -= (long long)r;
        }
    }
    free(coef);

    seq_t res = pos - neg;
    if (rs >= 0) res += (seq_t)(uint64_t)(rs / (long long)n);
    else res -= (seq_t)(uint64_t)((-rs) / (long long)n);
    return res;
}

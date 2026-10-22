/*
 * A004936  Numerator of (binomial(2*n-2,n-1)/n!)^2.
 * https://oeis.org/A004936
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = numerator( (binomial(2*n-2,n-1)/n!)^2 ) = prod_{p prime}
 *            p^(2*max(0, v_p(binomial(2n-2,n-1)) - v_p(n!))), using
 *            Legendre's formula for the p-adic valuations
 * Range:     u8 n <= 7, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 51,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n - 1;
    uint64_t lim = 2 * m;
    seq_t r = 1;
    if (lim < 2) return r;
    char *comp = (char *)calloc((size_t)(lim + 1), 1);
    for (uint64_t p = 2; p <= lim; p++) {
        if (comp[p]) continue;
        for (uint64_t j = p * p; j <= lim; j += p) comp[j] = 1;
        /* v_p(binomial(2m, m)) */
        long long vb = 0;
        uint64_t q = p;
        while (1) {
            vb += (long long)(lim / q) - 2 * (long long)(m / q);
            if (q > lim / p) break;
            q *= p;
        }
        /* v_p(n!) */
        long long vf = 0;
        q = p;
        while (q <= (uint64_t)n) {
            vf += (long long)((uint64_t)n / q);
            if (q > (uint64_t)n / p) break;
            q *= p;
        }
        long long e = vb - vf;
        for (long long k = 0; k < 2 * e; k++) r *= (seq_t)p;
    }
    free(comp);
    return r;
}

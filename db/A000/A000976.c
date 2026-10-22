/*
 * A000976  Period of 1/n! in base 10.
 * https://oeis.org/A000976
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = order(10, n!/(2^s*5^t)) = lcm over primes 2,5 != p <= n
 *            of ord_p(10) * p^max(0, e_p - v_p), where e_p = v_p(n!) and v_p
 *            = v_p(10^ord_p(10) - 1) (lifting the exponent); a(1) = a(2) = 0
 * Range:     u8 n <= 13, u16 n <= 17, u32 n <= 26, u64 n <= 38, u128 n <= 62,
 *            big any n
 * Helpers:   a976_powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a976_powmod.h"

seq_t a(idx_t n) {
    if (n <= 2) return 0;
    uint64_t N = (uint64_t)n;
    uint32_t *spf = (uint32_t *)calloc(N + 1, sizeof(uint32_t));
    uint64_t *ex = (uint64_t *)calloc(N + 1, sizeof(uint64_t));
    uint64_t i, j;
    for (i = 2; i <= N; i++) {
        if (spf[i] == 0) {
            for (j = i; j <= N; j += i)
                if (spf[j] == 0) spf[j] = (uint32_t)i;
        }
    }
    for (i = 3; i <= N; i++) {
        uint64_t p = i, e = 0, pk, d, m, t, pj;
        if (spf[p] != p || p == 5) continue;
        /* e = v_p(n!) */
        pk = p;
        while (1) {
            e += N / pk;
            if (pk > N / p) break;
            pk *= p;
        }
        /* d = ord_p(10) */
        d = p - 1;
        m = p - 1;
        while (m > 1) {
            uint64_t q = spf[m];
            while (m % q == 0) m /= q;
            while (d % q == 0 && a976_powmod(10, d / q, p) == 1) d /= q;
        }
        /* t = min(v_p(10^d - 1), e) */
        t = 1;
        pj = p;
        while (t < e && pj <= (UINT64_C(1) << 62) / p) {
            pj *= p;
            if (a976_powmod(10, d, pj) == 1) t++;
            else break;
        }
        /* record factorization of d * p^(e - t) */
        m = d;
        while (m > 1) {
            uint64_t q = spf[m], c = 0;
            while (m % q == 0) { m /= q; c++; }
            if (c > ex[q]) ex[q] = c;
        }
        if (e - t > ex[p]) ex[p] = e - t;
    }
    seq_t r = 1;
    for (i = 2; i <= N; i++) {
        uint64_t k;
        for (k = 0; k < ex[i]; k++) r *= (seq_t)i;
    }
    free(spf);
    free(ex);
    return r;
}

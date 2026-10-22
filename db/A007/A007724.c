/*
 * A007724  Even minus odd extensions of truncated 3 X 2n grid diagram.
 * https://oeis.org/A007724
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = multinomial(3n; n-1, n, n+1)/(n(2n-1)(2n+1)) =
 *            (3n)!/((n-1)!*n!*(n+1)!*n*(2n-1)*(2n+1)), evaluated via prime
 *            factorization (Legendre) so every partial product divides a(n)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 17, u128 n <= 20,
 *            big any n
 * Helpers:   val_p
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/val_p.h"

static uint64_t legendre_exp(uint64_t m, uint64_t p) {
    uint64_t e = 0;
    while (m) {
        m /= p;
        e += m;
    }
    return e;
}

seq_t a(idx_t n) {
    uint64_t N = 3 * (uint64_t)n;
    unsigned char *comp = (unsigned char *)calloc(N + 1, 1);
    seq_t r = 1;
    uint64_t p, q, k;
    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (q = p * p; q <= N; q += p) comp[q] = 1;
        {
            uint64_t num = legendre_exp(N, p);
            uint64_t den = legendre_exp(n - 1, p) + legendre_exp(n, p) + legendre_exp(n + 1, p)
                         + val_p(n, p) + val_p(2 * (uint64_t)n - 1, p) + val_p(2 * (uint64_t)n + 1, p);
            uint64_t e = num - den;
            for (k = 0; k < e; k++) r *= (seq_t)p;
        }
    }
    free(comp);
    return r;
}

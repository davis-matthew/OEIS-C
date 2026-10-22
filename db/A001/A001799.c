/*
 * A001799  Coefficient of P_4(x) in the Legendre expansion of x^n.
 * https://oeis.org/A001799
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = (9*(2*n)*(2*n-2)/((2*n+1)*(2*n+3)*(2*n+5))) *
 *            numerator(binomial(4*n, 2*n)/2^(4*n)), computed by prime
 *            factorization (Legendre's formula for the binomial, 2-part
 *            removed)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   a1799_addf
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a1799_addf.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t M = 4 * N + 6;
    int *e = (int *)calloc(M + 1, sizeof(int));
    unsigned char *comp = (unsigned char *)calloc(M + 1, 1);
    uint64_t i, j, p;
    seq_t r = 1;
    uint64_t acc = 1;

    a1799_addf(e, 36, 1);
    a1799_addf(e, N, 1);
    a1799_addf(e, N - 1, 1);
    a1799_addf(e, 2 * N + 1, -1);
    a1799_addf(e, 2 * N + 3, -1);
    a1799_addf(e, 2 * N + 5, -1);

    for (i = 2; i * i <= M; i++)
        if (!comp[i])
            for (j = i * i; j <= M; j += i) comp[j] = 1;

    /* odd primes: exponent of p in binomial(4n, 2n) */
    for (p = 3; p <= M; p += 2) {
        uint64_t q;
        if (comp[p]) continue;
        for (q = p; q <= 4 * N; q *= p) {
            e[p] += (int)((4 * N) / q - 2 * ((2 * N) / q));
            if (q > (4 * N) / p) break;
        }
    }

    for (p = 2; p <= M; p++) {
        int k;
        if (comp[p] && p > 3) continue;
        if (p >= 4 && comp[p]) continue;
        for (k = 0; k < e[p]; k++) {
            if (acc >= ((uint64_t)1 << 40)) {
                r *= (seq_t)acc;
                acc = 1;
            }
            acc *= p;
        }
    }
    r *= (seq_t)acc;

    free(e);
    free(comp);
    return r;
}

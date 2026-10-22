/*
 * A002296  Number of dissections of a polygon: binomial(7n,n)/(6n+1).
 * https://oeis.org/A002296
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = binomial(7*n,n)/(6*n+1) = (7n)! / (n! * (6n+1)!),
 *            evaluated as a product of primes p^e with e from Legendre's
 *            formula
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 33,
 *            big any n
 * Helpers:   a002296_leg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/a002296_leg.h"

seq_t a(idx_t n) {
    seq_t r = (seq_t)1;
    uint64_t N = 7 * (uint64_t)n;
    uint64_t M = 6 * (uint64_t)n + 1;
    if (n == 0) return r;
    char *comp = (char *)malloc((size_t)N + 1);
    memset(comp, 0, (size_t)N + 1);
    uint64_t acc = 1;
    for (uint64_t p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (uint64_t q = p * p; q <= N; q += p) comp[q] = 1;
        long long e = (long long)a002296_leg(N, p)
                    - (long long)a002296_leg((uint64_t)n, p)
                    - (long long)a002296_leg(M, p);
        while (e-- > 0) {
            if (acc >= ((uint64_t)1 << 32)) {
                r *= (seq_t)acc;
                acc = 1;
            }
            acc *= p;
        }
    }
    free(comp);
    r *= (seq_t)acc;
    return r;
}

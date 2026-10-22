/*
 * A002294  a(n) = binomial(5*n, n)/(4*n + 1).
 * https://oeis.org/A002294
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = binomial(5*n, n)/(4*n + 1) = (5n)!/(n! * (4n+1)!),
 *            evaluated by prime factorization (Legendre's formula)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   a2294_vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a2294_vfact.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    uint64_t N = 5 * (uint64_t)n;
    uint64_t M = 4 * (uint64_t)n + 1;
    unsigned char *comp = (unsigned char *)calloc((size_t)(N + 1), 1);
    seq_t r = 1;
    uint64_t p, q;
    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        for (q = p * p; q <= N; q += p) comp[q] = 1;
        uint64_t e = a2294_vfact(N, p) - a2294_vfact((uint64_t)n, p) - a2294_vfact(M, p);
        while (e > 0) {
            r *= (seq_t)p;
            e--;
        }
    }
    free(comp);
    return r;
}

/*
 * A002293  Number of dissections of a polygon: binomial(4*n, n)/(3*n + 1).
 * https://oeis.org/A002293
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = binomial(4*n,n)/(3*n+1) = (4n)!/(n!*(3n+1)!), computed by
 *            prime factorization (Legendre's formula)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 22, u128 n <= 42,
 *            big any n
 * Helpers:   a002293_leg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a002293_leg.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    uint64_t N, i, j, p;
    unsigned char *comp;
    if (n == 0) return r;
    N = 4 * (uint64_t)n;
    comp = (unsigned char *)calloc((size_t)N + 1, 1);
    for (i = 2; i <= N; i++) {
        if (comp[i]) continue;
        for (j = i * i; j <= N; j += i) comp[j] = 1;
        p = i;
        {
            uint64_t e = a002293_leg(N, p) - a002293_leg((uint64_t)n, p)
                       - a002293_leg(3 * (uint64_t)n + 1, p);
            while (e--) r *= (seq_t)p;
        }
    }
    free(comp);
    return r;
}

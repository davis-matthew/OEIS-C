/*
 * A001421  a(n) = (6*n)!/((n!)^3*(3*n)!).
 * https://oeis.org/A001421
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (6*n)!/((n!)^3*(3*n)!), computed as Product_{p prime <=
 *            6n} p^e(p), e(p) = v_p((6n)!) - 3*v_p(n!) - v_p((3n)!)
 *            (Legendre)
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   a001421_leg
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a001421_leg.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    uint64_t N, i, j, p, e, k, chunk;
    unsigned char *comp;
    if (n == 0) return r;
    N = 6 * (uint64_t)n;
    comp = (unsigned char *)calloc((size_t)(N + 1), 1);
    for (i = 2; i * i <= N; i++)
        if (!comp[i])
            for (j = i * i; j <= N; j += i) comp[j] = 1;
    chunk = 1;
    for (p = 2; p <= N; p++) {
        if (comp[p]) continue;
        e = a001421_leg(N, p) - 3 * a001421_leg((uint64_t)n, p) - a001421_leg(3 * (uint64_t)n, p);
        for (k = 0; k < e; k++) {
            if (chunk > (UINT64_C(1) << 32) / p) {
                r *= (seq_t)chunk;
                chunk = 1;
            }
            chunk *= p;
        }
    }
    if (chunk > 1) r *= (seq_t)chunk;
    free(comp);
    return r;
}

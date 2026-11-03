/*
 * A007160  Number of diagonal dissections of a convex (n+6)-gon into n
 *          regions.
 * https://oeis.org/A007160
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   a(n) = binomial(n+3,4)*binomial(2n+4,n-1)/n = (2n+4)! *
 *            (n+1)(n+2)(n+3) / (24 * (n-1)! * (n+5)!), evaluated by prime
 *            factorization
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 12, u64 n <= 26, u128 n <= 57,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 150 (2026-11-01)
 * Helpers:   valuation_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/valuation_u64.h"

static long long a007160_vfact(uint64_t m, uint64_t p) {
    long long e = 0;
    while (m) { m /= p; e += (long long)m; }
    return e;
}

seq_t a(idx_t n) {
    uint64_t lim = 2 * n + 4, i, j;
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    seq_t r = 1;
    for (i = 2; i <= lim; i++) {
        long long e;
        if (comp[i]) continue;
        for (j = i * i; j <= lim; j += i) comp[j] = 1;
        e = a007160_vfact(2 * n + 4, i)
          + valuation_u64(n + 1, i) + valuation_u64(n + 2, i) + valuation_u64(n + 3, i)
          - a007160_vfact(n - 1, i) - a007160_vfact(n + 5, i)
          - valuation_u64(24, i);
        while (e-- > 0) r *= (seq_t)i;
    }
    free(comp);
    return r;
}

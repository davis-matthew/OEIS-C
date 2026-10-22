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
 * Helpers:   a007160_v, a007160_vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a007160_v.h"
#include "../utils/a007160_vfact.h"

seq_t a(idx_t n) {
    uint64_t lim = 2 * n + 4, i, j;
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    seq_t r = 1;
    for (i = 2; i <= lim; i++) {
        long long e;
        if (comp[i]) continue;
        for (j = i * i; j <= lim; j += i) comp[j] = 1;
        e = a007160_vfact(2 * n + 4, i)
          + a007160_v(n + 1, i) + a007160_v(n + 2, i) + a007160_v(n + 3, i)
          - a007160_vfact(n - 1, i) - a007160_vfact(n + 5, i)
          - a007160_v(24, i);
        while (e-- > 0) r *= (seq_t)i;
    }
    free(comp);
    return r;
}

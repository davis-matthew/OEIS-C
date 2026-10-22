/*
 * A005017  Denominator of (binomial(2*n-2,n-1)/n!)^2.
 * https://oeis.org/A005017
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = denominator( (binomial(2*n-2,n-1)/n!)^2 ) = D^2, where D
 *            = Product_{p prime <= n} p^max(0, v_p(n!) -
 *            v_p(binomial(2*n-2,n-1)))
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 9, u64 n <= 15, u128 n <= 24,
 *            big any n
 * Helpers:   a005017_vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/a005017_vfact.h"

seq_t a(idx_t n) {
    seq_t d = 1;
    unsigned char *comp;
    idx_t i, j, p;
    if (n < 2) return 1;
    comp = (unsigned char *)malloc((size_t)n + 1);
    memset(comp, 0, (size_t)n + 1);
    for (i = 2; i * i <= n; i++)
        if (!comp[i])
            for (j = i * i; j <= n; j += i) comp[j] = 1;
    for (p = 2; p <= n; p++) {
        idx_t vf, vb, e, k;
        if (comp[p]) continue;
        vf = a005017_vfact(n, p);
        vb = a005017_vfact(2 * n - 2, p) - 2 * a005017_vfact(n - 1, p);
        if (vf <= vb) continue;
        e = vf - vb;
        for (k = 0; k < e; k++) d *= (seq_t)p;
    }
    free(comp);
    return d * d;
}

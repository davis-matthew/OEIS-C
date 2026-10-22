/*
 * A004677  Numerator of 2^n*(3*n-3)!/( ((n-1)!)^3 * (2*n)! ).
 * https://oeis.org/A004677
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   a(n) = numerator of 2^n*(3*n-3)!/( ((n-1)!)^3 * (2*n)! ),
 *            computed as prod_{p prime} p^max(0, e_p), where by Legendre's
 *            formula e_p = v_p(2^n) + v_p((3n-3)!) - 3*v_p((n-1)!) -
 *            v_p((2n)!)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 21, u64 n <= 34, u128 n <= 65,
 *            big any n
 * Helpers:   a004677_legendre
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/a004677_legendre.h"

seq_t a(idx_t n) {
    idx_t lim = 3 * n + 2;
    unsigned char *comp = (unsigned char *)calloc((size_t)lim + 1, 1);
    seq_t res = 1;
    idx_t p, q;
    for (p = 2; p <= lim; p++) {
        int64_t e;
        if (comp[p]) continue;
        for (q = p * p; q <= lim; q += p) comp[q] = 1;
        e = a004677_legendre(3 * n - 3, p)
            - 3 * a004677_legendre(n - 1, p)
            - a004677_legendre(2 * n, p);
        if (p == 2) e += (int64_t)n;
        while (e > 0) {
            res *= (seq_t)p;
            e--;
        }
    }
    free(comp);
    return res;
}

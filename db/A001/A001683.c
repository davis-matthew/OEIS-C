/*
 * A001683  Number of one-sided triangulations of the disk; or flexagons of
 *          order n; or unlabeled plane trivalent trees (n-2 internal
 *          vertices, all of degree 3 and hence n leaves).
 * https://oeis.org/A001683
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, nice, easy, changed
 * Formula:   a(n) = C(n-2)/n + C(n/2-1)/2 + (2/3)*C(n/3-1), C = Catalan
 *            (A000108), terms omitted if subscripts are not integers
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 24, u64 n <= 41, u128 n <= 74,
 *            big any n
 * Helpers:   a1683_cat_qr, a1683_leg, a1683_mulp
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a1683_cat_qr.h"
#include "../utils/a1683_leg.h"
#include "../utils/a1683_mulp.h"

seq_t a(idx_t n) {
    idx_t N = 2 * n + 2, i, j;
    unsigned char *comp = (unsigned char *)calloc((size_t)N + 1, 1);
    seq_t res, Q;
    idx_t R, F;

    for (i = 2; i * i <= N; i++)
        if (!comp[i])
            for (j = i * i; j <= N; j += i) comp[j] = 1;

    /* C(n-2)/n */
    a1683_cat_qr(n - 2, n, comp, &Q, &R);
    res = Q;
    F = 6 * R;

    /* C(n/2-1)/2 */
    if (n % 2 == 0) {
        a1683_cat_qr(n / 2 - 1, 2, comp, &Q, &R);
        res += Q;
        F += 3 * n * R;
    }

    /* 2*C(n/3-1)/3 */
    if (n % 3 == 0) {
        a1683_cat_qr(n / 3 - 1, 3, comp, &Q, &R);
        res += 2 * Q;
        F += 4 * n * R;
    }

    free(comp);
    res += (seq_t)(F / (6 * n));
    return res;
}

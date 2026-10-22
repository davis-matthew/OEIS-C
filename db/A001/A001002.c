/*
 * A001002  Number of dissections of a convex (n+2)-gon into triangles and
 *          quadrilaterals by nonintersecting diagonals.
 * https://oeis.org/A001002
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f. satisfies A(x) = 1 + x*A(x)^2 + x^2*A(x)^3, so a(m) =
 *            [x^(m-1)] A^2 + [x^(m-2)] A^3, a(0) = 1 (used for fixed-width
 *            seq_t, wraps exactly); for arbitrary precision: 5*n*(n+1)*a(n) =
 *            11*n*(2*n-1)*a(n-1) + 3*(3*n-2)*(3*n-4)*a(n-2), a(0) = 1, a(1) =
 *            1
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 29, u128 n <= 56,
 *            big any n
 * Helpers:   a001002_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a001002_is_big.h"

seq_t a(idx_t n) {
    if (n < 2) return 1;
    if (a001002_is_big()) {
        seq_t p = 1, q = 1;
        idx_t m;
        for (m = 2; m <= n; m++) {
            seq_t r = (q * (11 * m * (2 * m - 1)) + p * (3 * (3 * m - 2) * (3 * m - 4))) / (5 * m * (m + 1));
            p = q;
            q = r;
        }
        return q;
    } else {
        /* any term beyond this exceeds 128 bits */
        if (n > 200) return 0;
        {
            idx_t len = n + 1, m, i;
            seq_t *A = (seq_t *)malloc(len * sizeof(seq_t));
            seq_t *S = (seq_t *)malloc(len * sizeof(seq_t));
            seq_t res;
            A[0] = 1;
            S[0] = 1;
            for (m = 1; m <= n; m++) {
                seq_t s = 0, t = 0;
                /* S[m-1] = sum A[i]*A[m-1-i] */
                for (i = 0; i <= m - 1; i++) s += A[i] * 1u * A[m - 1 - i];
                S[m - 1] = s;
                if (m >= 2) {
                    for (i = 0; i <= m - 2; i++) t += A[i] * 1u * S[m - 2 - i];
                }
                A[m] = s + t;
            }
            res = A[n];
            free(A);
            free(S);
            return res;
        }
    }
}

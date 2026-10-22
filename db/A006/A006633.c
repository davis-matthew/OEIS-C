/*
 * A006633  Expansion of hypergeom([3/2, 7/4, 2, 9/4], [7/3, 8/3, 3],
 *          (256/27)*x).
 * https://oeis.org/A006633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(4*n+5, n)/(n+2), computed via a(n) = a(n-1) *
 *            4*(4n+5)*(4n+3)*(4n+2)*(n+1) / (3*n*(3n+5)*(3n+4)*(n+2)), a(0) =
 *            1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 21, u128 n <= 41,
 *            big any n
 * Helpers:   a006633_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006633_gcd.h"

seq_t a(idx_t n) {
    seq_t r = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        uint64_t nf[5], df[4];
        uint64_t N = 1, D = 1;
        int i, j;
        nf[0] = 4; nf[1] = 4 * k + 5; nf[2] = 4 * k + 3; nf[3] = 4 * k + 2; nf[4] = k + 1;
        df[0] = 3 * k; df[1] = 3 * k + 5; df[2] = 3 * k + 4; df[3] = k + 2;
        for (i = 0; i < 5; i++)
            for (j = 0; j < 4; j++) {
                uint64_t g = a006633_gcd(nf[i], df[j]);
                if (g > 1) { nf[i] /= g; df[j] /= g; }
            }
        for (i = 0; i < 5; i++) N *= nf[i];
        for (j = 0; j < 4; j++) D *= df[j];
        /* N/D is in lowest terms and a(k) is an integer, so D divides a(k-1) */
        r = (r / (seq_t)D) * (seq_t)N;
    }
    return r;
}

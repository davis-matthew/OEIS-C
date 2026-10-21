/*
 * A227485  The hyper-Wiener index of the dendrimer D_4[n], defined
 *          pictorially in the A. R. Ashrafi et al. reference.
 * https://oeis.org/A227485
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1007415 + 2^n*(-4509545 + 732099*n - 171585*n^2) +
 *            4^n*(3503295 -2446768*n + 864900*n^2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 4, u64 n <= 18, u128 n <= 48,
 *            big any n
 * Helpers:   pow2, pow4
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/pow4.h"

seq_t a(idx_t n) {
    /* Compute the quadratic polynomials using signed 64‑bit arithmetic. */
    int64_t nn = (int64_t)n;
    int64_t poly1 = -4509545LL + 732099LL * nn - 171585LL * nn * nn;
    int64_t poly2 = 3503295LL - 2446768LL * nn + 864900LL * nn * nn;

    /* Base value. */
    seq_t result = (seq_t)1007415;

    /* Powers of 2 and 4. */
    seq_t pow2n = pow2(n);
    seq_t pow4n = pow4(n);

    /* Add or subtract the contribution of the 4^n term. */
    if (poly2 >= 0) {
        result += pow4n * (seq_t)poly2;
    } else {
        result -= pow4n * (seq_t)(-poly2);
    }

    /* Add or subtract the contribution of the 2^n term. */
    if (poly1 >= 0) {
        result += pow2n * (seq_t)poly1;
    } else {
        result -= pow2n * (seq_t)(-poly1);
    }

    return result;
}

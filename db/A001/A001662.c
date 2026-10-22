/*
 * A001662  Coefficients of Airey's converging factor.
 * https://oeis.org/A001662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   b(n+1) = -2*n*b(n) + Sum_{k=0..n-1} binomial(n,k)*b(k+1)*b(n-k),
 *            b(0)=0, b(1)=1 (from exp(B(x)) = 1 + 2*x - B(x) with B(x) =
 *            2*Sum b(n)*(x/2)^n/n!); a(0) = 0, a(1) = b(1) = 1, a(n) = -b(n)
 *            for n >= 2
 * Range:     i8 n <= 7, i16 n <= 10, i32 n <= 13, i64 n <= 21, i128 n <= 35,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    seq_t *b = alloc_zero(n + 1);
    seq_t *C = alloc_zero(n + 1);
    b[1] = 1;
    C[0] = 1;
    for (idx_t m = 1; m < n; m++) {
        /* advance Pascal row to row m */
        for (idx_t k = m; k >= 1; k--) C[k] += C[k - 1];
        seq_t s = 0;
        s -= b[m] * (seq_t)m * 2;
        for (idx_t k = 0; k < m; k++) {
            s += C[k] * b[k + 1] * b[m - k];
        }
        b[m + 1] = s;
    }
    seq_t r;
    if (n == 1) r = b[1];
    else r = 0 - b[n];
    release(b, n + 1);
    release(C, n + 1);
    return r;
}

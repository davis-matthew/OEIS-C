/*
 * A001497  Triangle of coefficients of Bessel polynomials (exponents in
 *          decreasing order).
 * https://oeis.org/A001497
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   T(n,m) = (2*n-m-1)*T(n-1,m) + T(n-1,m-1), T(0,0) = 1, T(n,m) = 0
 *            for m > n or m < 0; a(k) is the triangle read by rows, so k =
 *            n*(n+1)/2 + m
 * Range:     u8 n <= 14, u16 n <= 27, u32 n <= 65, u64 n <= 170,
 *            u128 n <= 434, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    idx_t row = 0;
    idx_t idx = 0;
    while (idx + row + 1 <= n) {
        idx += row + 1;
        row++;
    }
    idx_t col = n - idx;

    seq_t *t = (seq_t *)malloc((size_t)(row + 1) * sizeof(seq_t));
    idx_t c, r;
    for (c = 0; c <= row; c++) t[c] = 0;
    t[0] = 1;

    for (r = 1; r <= row; r++) {
        /* update in place from high column to low */
        for (c = r; ; c--) {
            seq_t term1 = (c < r) ? t[c] : (seq_t)0;
            seq_t term2 = (c > 0) ? t[c - 1] : (seq_t)0;
            seq_t coeff = (seq_t)(2 * r - c - 1);
            t[c] = coeff * term1 + term2;
            if (c == 0) break;
        }
    }

    seq_t result = t[col];
    free(t);
    return result;
}

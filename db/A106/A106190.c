/*
 * A106190  Triangle read by rows: T(n,k) = binomial(2(n-k),n-k)/(1-2(n-k)).
 * https://oeis.org/A106190
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign, tabl
 * Formula:   a(n) = binomial(2*d, d) / (1 - 2*d), where r = floor((sqrt(8*n +
 *            1) - 1) / 2) is the row number, c = n - r*(r + 1)/2 is the
 *            column number, d = r - c is the distance from the diagonal
 * Range:     i8 n <= 14, i16 n <= 44, i32 n <= 69, i64 n <= 69, i128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-01)
 * Helpers:   central_binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/central_binomial.h"

seq_t a(idx_t n) {
    /* Find the row r such that r(r+1)/2 <= n < (r+1)(r+2)/2. */
    idx_t r = 0;
    {
        __int128 tmp = (__int128)8 * n + 1;
        double root = sqrt((double)tmp);
        r = (idx_t)((root - 1.0) / 2.0);
        /* Correct possible rounding errors. */
        while ((r + 1) * (r + 2) / 2 <= n) ++r;
        while (r * (r + 1) / 2 > n) --r;
    }

    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;
    idx_t d = r - c;        /* distance from the diagonal */

    seq_t denom = (seq_t)1 - (seq_t)(2 * d);
    seq_t num = central_binomial(d);
    return num / denom;
}

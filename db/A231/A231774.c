/*
 * A231774  Triangular array read by rows: row n shows the coefficients of the
 *          polynomial u(n) = c(0) + c(1)*x + ... + c(n)*x^(n) which is the
 *          numerator of the n-th convergent of the continued fraction [k, k,
 *          k, ... ], where k = (x + 1)/(x + 2).
 * https://oeis.org/A231774
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the coefficient of x^p in the (r)-th row (r >= 1) of the
 *            * triangular array of numerators P_r of the finite continued
 *            fraction * [k, k, …] with k = (x + 1)/(x + 2); row 0 is the
 *            special polynomial x + 2
 * Range:     u8 n <= 16, u16 n <= 51, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   delete_poly, new_poly
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/delete_poly.h"
#include "../utils/new_poly.h"

seq_t a(idx_t n) {
    /* Find the row number r (0‑based) and the position p in that row. */
    idx_t r = 0;
    idx_t cum = 0;                     /* number of terms processed so far */
    while (1) {
        idx_t len = r + 2;              /* each row has r+2 coefficients */
        if (n <= cum + len) break;     /* n lies in this row */
        cum += len;
        ++r;
    }
    idx_t p = n - cum - 1;             /* 0‑based index within the row */

    /* Special handling for row 0: coefficients of x + 2. */
    if (r == 0) {
        return (p == 0) ? (seq_t)2 : (seq_t)1;
    }

    /* P_-1 = 1, P_0 = x + 1  (coefficients [1, 1]) */
    size_t len2 = 1;                   /* degree of P_-1 is 0 */
    seq_t* prev2 = new_poly(len2);
    prev2[0] = (seq_t)1;

    size_t len1 = 2;                   /* degree of P_0 is 1 */
    seq_t* prev1 = new_poly(len1);
    prev1[0] = (seq_t)1;               /* constant term */
    prev1[1] = (seq_t)1;               /* coefficient of x */

    /* Iterate to compute P_r. */
    size_t i;
    for (i = 1; i <= (size_t)r; ++i) {
        size_t cur_len = i + 2;        /* degree is i+1, so len = i+2 */
        seq_t* cur = new_poly(cur_len);

        /* Zero-initialize cur (necessary for C where new[] may not zero). */
        size_t z;
        for (z = 0; z < cur_len; ++z) cur[z] = (seq_t)0;

        /* Add (x + 1) * prev1. */
        for (z = 0; z < len1; ++z) {
            cur[z]     += prev1[z];
            cur[z + 1] += prev1[z];
        }

        /* Add (x + 2)^2 * prev2  ( (x^2 + 4x + 4) * prev2 ). */
        for (z = 0; z < len2; ++z) {
            seq_t v = prev2[z];
            cur[z]     += v * (seq_t)4;
            cur[z + 1] += v * (seq_t)4;
            cur[z + 2] += v;
        }

        /* Advance the rolling arrays. */
        delete_poly(prev2);
        prev2 = prev1;
        len2 = len1;
        prev1 = cur;
        len1 = cur_len;
    }

    /* prev1 now holds P_r.  Extract the requested coefficient. */
    seq_t result = prev1[p];

    delete_poly(prev2);
    delete_poly(prev1);

    return result;
}

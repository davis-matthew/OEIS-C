/*
 * A137514  Triangle of coefficients of row polynomials with e.g.f. p(x,t) =
 *          exp(x*t)*(1 - 6*t + 9*t^2 - 4*t^3 + t^4)/(4*t - 1)/(2*t - 1).
 * https://oeis.org/A137514
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = fact(row)/fact(col) * r[row-col] * where row is the
 *            smallest integer with (row+1)(row+2)/2 >= n, * col = n -
 *            row*(row+1)/2 - 1, * and r_j are the coefficients of the series
 *            * R(t) = (1-6t+9t^2-4t^3+t^4)/((1-4t)(1-2t))
 * Range:     u8 n <= 15, u16 n <= 28, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* pre‑computed tables, initialized once */
    static int initialized = 0;
    /* maximum row needed for n up to 10000 (row ≈ 141) */
    /* we use a fixed size large enough for that range */
    enum { MAX_ROW = 200 };
    static seq_t fact[MAX_ROW + 1];
    static seq_t q[MAX_ROW + 1];
    static seq_t rcoeff[MAX_ROW + 1];

    if (!initialized) {
        /* factorials */
        fact[0] = (seq_t)1;
        for (idx_t i = 1; i <= MAX_ROW; ++i)
            fact[i] = fact[i - 1] * (seq_t)i;

        /* q_n from 1/(1-6t+8t^2) with recurrence q_n = 6q_{n-1} - 8q_{n-2} */
        q[0] = (seq_t)1;
        for (idx_t i = 1; i <= MAX_ROW; ++i) {
            seq_t t1 = (seq_t)6 * q[i - 1];
            seq_t t2 = (i >= 2) ? (seq_t)8 * q[i - 2] : (seq_t)0;
            q[i] = t1 - t2;                 /* q[i] is positive for all i >= 0 */
        }

        /* coefficients of P(t) = 1 - 6t + 9t^2 - 4t^3 + t^4 */
        const int64_t p_coef[5] = {1, -6, 9, -4, 1};
        for (idx_t j = 0; j <= MAX_ROW; ++j) {
            int64_t sum = 0;
            for (int k = 0; k <= 4 && (idx_t)k <= j; ++k)
                sum += p_coef[k] * (int64_t)q[j - k];
            rcoeff[j] = (seq_t)sum;          /* rcoeff[j] is non‑negative */
        }

        initialized = 1;
    }

    /* locate the row and column in the triangle that correspond to index n */
    idx_t row = 0;    /* row index, 0‑based */
    while (((row + 1) * (row + 2)) / 2 < n)
        ++row;

    idx_t s_prev = row * (row + 1) / 2;          /* elements before this row */
    idx_t col = n - s_prev - 1;                 /* column in the row (0‑based) */
    idx_t j = row - col;                        /* index for rcoeff */

    /* compute fact(row)/fact(col) * rcoeff[j] */
    seq_t result = (seq_t)1;
    for (idx_t k = col + 1; k <= row; ++k)
        result = result * (seq_t)k;
    result = result * rcoeff[j];

    return result;
}

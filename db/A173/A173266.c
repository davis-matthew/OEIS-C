/*
 * A173266  T(0,k) = 1 and T(n,k) = [x^k] (x^(n + 1) - 1)/((x - 2)*x^n + 1)
 *          for n >= 1, square array read by descending antidiagonals (n >= 0,
 *          k >= 0).
 * https://oeis.org/A173266
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   a(n) = coefficient of x^k in (x^(n+1)–1)/((x–2)*x^n+1) read by
 *            descending antidiagonals of T(n,k)
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Map linear index n to coordinates (row, col) in the square array T. */
    idx_t d = 0;                 /* current diagonal index */
    idx_t count = 1;             /* number of elements up to and including diagonal d */
    while (count <= n) {
        d++;
        count = (d + 1) * (d + 2) / 2;
    }
    idx_t prev = d * (d + 1) / 2;    /* elements before diagonal d */
    idx_t p = n - prev;              /* position inside diagonal d */
    idx_t row = p;                   /* n-coordinate of T */
    idx_t col = d - p;               /* k-coordinate of T */

    if (row == 0) return 1;           /* T(0,k) = 1 */

    /* Compute T(row, col) via the recurrence derived from
       (x^(row+1) – 1) = (x^(row+1) – 2x^row + 1) * Sum_{k>=0} T(row,k) x^k   */
    idx_t k = col;
    size_t sz = (size_t)(k + 1);
    seq_t *coef = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!coef) return 0;           /* unlikely failure */

    for (size_t i = 0; i < sz; ++i) coef[i] = 0;

    for (idx_t e = 0; e <= k; ++e) {
        seq_t en = (e >= row)     ? coef[e - row]     : 0;
        seq_t en1 = (e >= row + 1) ? coef[e - row - 1] : 0;
        seq_t b;

        if (e == 0)               b = -1;
        else if (e == row + 1)    b = 1;
        else                      b = 0;

        coef[e] = b + 2 * en - en1;
    }

    seq_t result = coef[k];
    free(coef);
    return result;
}

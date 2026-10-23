/*
 * A144089  T(n,k) is the number of partial bijections (or subpermutations) of
 *          an n-element set of height k (height(alpha) = |Im(alpha)|) and
 *          without fixed points.
 * https://oeis.org/A144089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nice, nonn, tabl
 * Formula:   a(n) = T(n,k) where n is the linear index of the triangle
 *            (row-major) of partial bijections without fixed points. (OEIS
 *            A144089) T(row, col) = (row!/(row-col)!) * Σ_{m=0}^{col} (-1)^m
 *            / m! * binom(row-m, col-m). The function below unflattens the
 *            index and evaluates this formula
 * Range:     u8 n <= 15, u16 n <= 29, u32 n <= 51, u64 n <= 51, u128 n <= 51,
 *            big any n
 * Helpers:   binom, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"      /* binom(idx_t a, idx_t b) -> seq_t */
#include "../utils/factorial.h"  /* factorial(idx_t n) -> seq_t */

static idx_t triangle_row(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    idx_t row   = triangle_row(n);               /* row index, 0‑based */
    idx_t offset = row * (row + 1) / 2;          /* start index of this row */
    idx_t col   = n - offset;                    /* column index in the row */

    seq_t fact_row   = factorial(row);           /* factorial(row) */
    seq_t fact_rowk  = factorial(row - col);     /* factorial(row-col) */

    seq_t sum = 0;

    for (idx_t m = 0; m <= col; ++m) {
        seq_t bin = binom(row - m, col - m);        /* binom(row-m, col-m) */
        seq_t term = bin * fact_row / factorial(m); /* incorporate (row! / m!) */
        if ((m & 1u) == 0)
            sum += term;                          /* even m: add */
        else
            sum -= term;                          /* odd m: subtract */
    }

    return sum / fact_rowk;                        /* divide by (row-col)! */
}

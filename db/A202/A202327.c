/*
 * A202327  Triangle read by rows, T(n, k) is the coefficient of x^n in
 *          expansion of ((-1 - x + sqrt(1 + 2*x + 5*x^2)) /2)^k.
 * https://oeis.org/A202327
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) is the n‑th term read from the triangle T(n,k) =
 *            coefficient of x^n in ((-1 - x + sqrt(1 + 2*x +
 *            5*x^2))/(2*x))^k. The triangle is read row by row, i.e.
 *            a(1)=T(1,1), a(2)=T(2,1), a(3)=T(2,2), a(4)=T(3,1), a(5)=T(3,2),
 *            a(6)=T(3,3), …
 * Range:     i8 n <= 47, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static idx_t max_row = 0;        /* largest row that has been precomputed   */
static seq_t *b      = NULL;     /* coefficients of B(x)  (b[1..R])         */
static seq_t **powers = NULL;    /* powers[k][n] = coefficient of x^n in B^k */

/*  Find the row index r such that n lies in row r (S_{r-1} < n ≤ S_r)  */
static idx_t find_row(idx_t n)
{
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n) ++r;   /* r is zero‑based  */
    return r + 1;                            /* convert to 1‑based */
}

/*  Pre–compute B coefficient array and all needed powers up to row R  */
static void recompute_all(idx_t R)
{
    /*  Allocate / resize b[0..R]  */
    seq_t *new_b = (seq_t *)realloc(b, (R + 1) * sizeof(seq_t));
    if (new_b == NULL) exit(1);
    b = new_b;

    b[0] = 0;          /* unused – set to zero for safety */
    if (R >= 1) b[1] = 1;

    for (idx_t m = 2; m <= R; ++m) {
        seq_t sum = 0;
        for (idx_t i = 1; i <= m - 2; ++i) {
            sum += b[i] * b[m - 1 - i];
        }
        b[m] = -b[m - 1] - sum;
    }

    /*  Allocate / resize powers[0..R]  */
    seq_t **new_pow = (seq_t **)realloc(powers, (R + 1) * sizeof(seq_t *));
    if (new_pow == NULL) exit(1);
    powers = new_pow;

    /*  powers[0] : B^0 = 1  */
    seq_t *p0 = (seq_t *)calloc(R + 1, sizeof(seq_t));
    if (p0 == NULL) exit(1);
    p0[0] = 1;        /* coefficient of x^0 */
    powers[0] = p0;

    /*  Compute powers[k] for k = 1..R  */
    for (idx_t k = 1; k <= R; ++k) {
        seq_t *cur = (seq_t *)calloc(R + 1, sizeof(seq_t));
        if (cur == NULL) exit(1);

        for (idx_t i = 0; i <= R; ++i) {
            seq_t prev = powers[k - 1][i];
            if (prev == 0) continue;
            for (idx_t j = 1; j <= R - i; ++j) {
                cur[i + j] += prev * b[j];
            }
        }
        powers[k] = cur;
    }

    max_row = R;
}

/*  Main function required by the framework  */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined, return zero */

    idx_t r = find_row(n);                /* row number   (1‑based)    */
    idx_t k = n - r * (r - 1) / 2;        /* column index (1‑based)    */

    if (r > max_row) recompute_all(r);    /* ensure data exists   */

    return powers[k][r];                  /* coefficient of x^r in B^k */
}

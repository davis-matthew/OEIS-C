/*
 * A169656  Triangle, read by rows, T(n, k) = (-1)^n*(n!/k!)^2*binomial(n-1,
 *          k-1).
 * https://oeis.org/A169656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) reads the triangle T(row,col) defined by T(n,k) = (-1)^n *
 *            (n!/k!)^2 * binomial(n-1, k-1) (1 ≤ k ≤ n)
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 29, i64 n <= 78, i128 n <= 210,
 *            big any n
 * Helpers:   ratio_sq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ratio_sq.h"
/* ---------- utility: binomial coefficient (multiplicative) ---------- */
static seq_t binom_small(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k > n - k) k = n - k;
    seq_t r = 1;
    for (idx_t i = 1; i <= k; ++i) {
        r *= (seq_t)(n - k + i);
        r /= (seq_t)i;
    }
    return r;
}

/* --------------------------------------------------------------------- */
/*  Computes the n-th term of the flattened A169656 triangle.            */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Find the row 'row' such that 1+2+…+row ≥ n */
    idx_t row = 1;
    idx_t cum = 1;          /* sum of first 'row' integers */
    while (cum < n) {
        ++row;
        cum += row;
    }

    /* Compute column index (1-based) within the row */
    idx_t prev_cum = cum - row;
    idx_t col = n - prev_cum;      /* 1 ≤ col ≤ row */

    /* Compute (row!/col!)^2 */
    seq_t val = ratio_sq(row, col);

    /* Multiply by binomial(row-1, col-1) */
    val *= binom_small(row - 1, col - 1);

    /* Apply the sign (-1)^row */
    if (row & 1)
        val = -val;

    return val;
}

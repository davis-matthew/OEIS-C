/*
 * A115513  Inverse of matrix (1,x)+(x,x^3).
 * https://oeis.org/A115513
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 118, i16 n <= 118, i32 n <= 118, i64 n <= 118,
 *            i128 n <= 118, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A115513 – flattened inverse of the matrix
 *
 *  L = I + B  where  B_{r,c} = 1  iff  r == 3*c + 1  (otherwise 0).
 *  The inverse L⁻¹ is unit–lower triangular; its entries are
 *
 *        L⁻¹[r][c] = 1                                 if r == c                         (1)
 *                   = (-1)^m                            if r = 3^m * c + (3^m-1)/2       (2)
 *                   = 0                                 otherwise.
 *
 *  The OEIS sequence is the row–major flattening of this triangle:
 *    a(n) = L⁻¹[row][col]  where  row = floor((√(8n+1)-1)/2)
 *          and col = n - row*(row+1)/2.
 *
 *  This implementation follows the explicit formula (1)-(2).
 */

#include <stdint.h>

/* Helper to find the row index r for a given flattened index n.
 * r satisfies r*(r+1)/2 <= n < (r+1)*(r+2)/2.
 */
static idx_t _row_from_index(idx_t n)
{
    idx_t lo = 0, hi = n, r = 0;
    while (lo <= hi) {
        idx_t mid = (lo + hi) >> 1;
        idx_t tri = mid * (mid + 1) / 2;
        if (tri == n) { r = mid; break; }
        if (tri < n)  { r = mid; lo = mid + 1; }
        else          { hi = mid - 1; }
    }
    return r;
}

seq_t a(idx_t n)
{
    /* 1. Recover (row, col) from the flattened index n */
    idx_t r = _row_from_index(n);
    idx_t c = n - r * (r + 1) / 2;

    /* 2. On the diagonal */
    if (r == c) return (seq_t)1;

    /* 3. Look for an exponent m such that
     *        r = 3^m * c + (3^m - 1)/2
     */
    idx_t numerator = 2 * r + 1;
    idx_t denom     = 1 + 2 * c;

    if (numerator % denom != 0) return (seq_t)0;
    idx_t p   = numerator / denom;   /* p = 3^m  */
    if (p <= 1) return (seq_t)0;      /* m >= 1 is required for off–diagonal */

    /* 4. Check that p is a power of 3 and find the exponent m */
    int m = 0;
    while (p % 3 == 0) {
        p /= 3;
        ++m;
    }
    if (p != 1) return (seq_t)0;      /* not a power of 3 */

    /* 5. Return (-1)^m */
    return (m & 1) ? (seq_t)-1 : (seq_t)1;
}

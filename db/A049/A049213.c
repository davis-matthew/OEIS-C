/*
 * A049213  A convolution triangle of numbers obtained from A025749.
 * https://oeis.org/A049213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the nth term of the convolution triangle of A025749,
 *            defined by a(n,m) = [4*(4*(n-1)-m)*a(n-1,m) + m*a(n-1,m-1)] / n
 *            for n >= m >= 1, with a(n,0)=0, a(n,m)=0 when n<m, and a(1,1)=1.
 *            The sequence a(n) listed in OEIS is the triangle read row‑wise
 *            (triangular number indexing). This implement...
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAX_R 150     /* enough for n up to ≈10 000 (triangular number ≈150) */

static seq_t triangle[MAX_R + 1][MAX_R + 1];
static idx_t max_computed_row = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* offset is 1, but guard against 0 */

    /* Find row r and column c of the nth triangle entry (row‑wise) */
    idx_t row = 1;
    idx_t tri = 1;          /* cumulative triangular number */
    while (tri < n) {
        ++row;
        tri += row;         /* tri = row*(row+1)/2 */
    }
    idx_t col = n - (tri - row);   /* 1‑based column index */

    /* Initialise the first triangle entry once */
    if (max_computed_row == 0) {
        triangle[1][1] = (seq_t)1;
        max_computed_row = 1;
    }

    /* Compute triangle rows up to the needed row */
    for (idx_t r = max_computed_row + 1; r <= row; ++r) {
        for (idx_t c = 1; c <= r; ++c) {
            seq_t term1 = 0, term2 = 0;

            if (c <= r - 1) {
                /* 4*(4*(r-1)-c)*a(r-1,c) */
                term1 = (seq_t)4 * ((seq_t)4 * (seq_t)(r - 1) - (seq_t)c)
                      * triangle[r - 1][c];
            }
            if (c >= 2 && c - 1 <= r - 1) {
                /* c * a(r-1,c-1) */
                term2 = (seq_t)c * triangle[r - 1][c - 1];
            }
            /* a(r,c) = (term1 + term2) / r  – exact division */
            triangle[r][c] = (term1 + term2) / (seq_t)r;
        }
    }
    max_computed_row = row;

    return triangle[row][col];
}

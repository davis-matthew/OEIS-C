/*
 * A219236  Coefficient array for the third power of the monic integer
 *          Chebyshev polynomials 2*T(2*n,x/2) as a function of x^2.
 * https://oeis.org/A219236
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabf
 * Formula:   a(n) is the flattened coefficient array a(r,m) from OEIS
 *            A219236, i.e. the coefficient of x^(2*m) in (2*T(2*r,x/2))^3
 *            where r is the row and m the column corresponding to index n
 * Range:     i8 n <= 13, i16 n <= 38, i32 n <= 50, i64 n <= 50, i128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ==================================================================== */
#include <stddef.h>
#include <stdint.h>

/* constants ---------------------------------------------------------- */
#define MAX_R_CAP    200   /* enough for n up to at least 10000 */
#define MAX_POLY_CAP 512
#define MAX_ROW_LEN  (3*MAX_R_CAP + 1)

/* Chebyshev polynomials P_k(x) = 2*T(k,x/2) ------------------------- */
static seq_t poly[MAX_POLY_CAP][MAX_POLY_CAP];
static size_t poly_len[MAX_POLY_CAP];
static size_t polys_count = 0;

/* coefficient rows a(r,m) --------------------------------------------*/
static seq_t rows[MAX_R_CAP][MAX_ROW_LEN];
static size_t row_len[MAX_R_CAP];
static size_t rows_count = 0;

/* forward declarations ----------------------------------------------*/
static void ensure_polys(size_t kmax);
static void ensure_rows(size_t rmax);

/* build Chebyshev polynomials ---------------------------------------*/
static void ensure_polys(size_t kmax)
{
    if (polys_count == 0) {
        /* P_0(x) = 2, P_1(x) = x */
        poly_len[0] = 1;
        poly[0][0] = (seq_t)2;
        poly_len[1] = 2;
        poly[1][0] = (seq_t)0;
        poly[1][1] = (seq_t)1;
        polys_count = 2;
    }
    while (polys_count <= kmax) {
        size_t k = polys_count;          /* new polynomial index    */
        poly_len[k] = k + 1;
        for (size_t i = 0; i < poly_len[k]; ++i)
            poly[k][i] = (seq_t)0;
        /* x * P_{k-1}  */
        for (size_t i = 0; i < poly_len[k-1]; ++i)
            poly[k][i+1] = poly[k-1][i];
        /* - P_{k-2}   */
        for (size_t i = 0; i < poly_len[k-2]; ++i)
            poly[k][i] -= poly[k-2][i];
        ++polys_count;
    }
}

/* build coefficient rows up to rmax ---------------------------------*/
static void ensure_rows(size_t rmax)
{
    while (rows_count <= rmax) {
        size_t r = rows_count;
        ensure_polys(2*r);                /* need P_{2r} */

        size_t arr_len = r + 1;
        seq_t arr[512];                    /* temporary even coeffs */
        for (size_t i = 0; i < arr_len; ++i)
            arr[i] = poly[2*r][2*i];

        size_t out_len = 3*r + 1;
        for (size_t i = 0; i < out_len; ++i)
            rows[r][i] = (seq_t)0;

        /* triple convolution */
        for (size_t i = 0; i < arr_len; ++i) {
            seq_t ai = arr[i];
            for (size_t j = 0; j < arr_len; ++j) {
                seq_t mul = ai * arr[j];
                for (size_t k = 0; k < arr_len; ++k) {
                    rows[r][i + j + k] += mul * arr[k];
                }
            }
        }

        row_len[r] = out_len;
        ++rows_count;
    }
}

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* find the triangular row r that contains index n            */
    size_t r = 0;
    while (1) {
        seq_t total = (seq_t)(r + 1) * (seq_t)(3*r + 2) / (seq_t)2;
        if ((idx_t)n < (idx_t)total)
            break;
        ++r;
    }

    /* column m within that row */
    size_t m;
    if (r == 0)
        m = 0;
    else
        m = (size_t)n - (size_t)(r * (3*r - 1) / 2);

    ensure_rows(r);
    return rows[r][m];
}

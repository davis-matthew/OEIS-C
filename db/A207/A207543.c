/*
 * A207543  Triangle read by rows, expansion of (1+y*x)/(1-2*y*x+y*(y-1)*x^2).
 * https://oeis.org/A207543
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the nth entry of the triangle read by rows, where the
 *            triangle T satisfies T(0,0)=1, T(1,0)=0, T(1,1)=3,
 *            T(n,k)=2*T(n-1,k-1)+T(n-2,k-1)-T(n-2,k-2) (for n≥2, 0≤k≤n), and
 *            a(n)=T(r,k) with r chosen so that r(r+1)/2 ≤ n < (r+1)(r+2)/2
 *            and k = n - r(r+1)/2
 * Range:     u8 n <= 51, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>           /* malloc, realloc, calloc */
#include <math.h>             /* sqrt */

static seq_t **triangle = NULL;   /* rows of the triangle */
static size_t triangle_rows = 0;   /* number of rows already computed */

/* Ensure that rows 0..upto are stored in the global caches. */
static void ensure_rows(idx_t upto)
{
    /* Expand the row pointer array if necessary */
    if (triangle == NULL) {
        triangle = (seq_t **)malloc(sizeof(seq_t *) * (upto + 1));
        if (!triangle) return;   /* out of memory – the build will abort */
    } else if (triangle_rows <= (size_t)upto) {
        seq_t **tmp = (seq_t **)realloc(triangle,
                                        sizeof(seq_t *) * (upto + 1));
        if (!tmp) return;
        triangle = tmp;
    }

    /* Compute any new rows that are missing */
    for (size_t i = triangle_rows; i <= (size_t)upto; ++i) {
        triangle[i] = (seq_t *)calloc((i + 1), sizeof(seq_t));
        if (!triangle[i]) return;

        if (i == 0) {
            triangle[0][0] = (seq_t)1;    /* T(0,0) */
        } else if (i == 1) {
            triangle[1][0] = (seq_t)0;    /* T(1,0) */
            triangle[1][1] = (seq_t)3;    /* T(1,1) */
        } else {
            triangle[i][0] = (seq_t)0;    /* T(i,0)=0 for i>0 */
            for (size_t k = 1; k <= i; ++k) {
                seq_t val = (seq_t)0;
                /* 2*T(n-1,k-1) */
                val += (seq_t)2 * triangle[i - 1][k - 1];
                /* + T(n-2,k-1) if defined */
                if (k <= i - 1) /* k-1 ≤ i-2 ⇒ k ≤ i-1 */
                    val += triangle[i - 2][k - 1];
                /* - T(n-2,k-2) if k≥2 (then k-2≤i-2 always) */
                if (k >= 2)
                    val -= triangle[i - 2][k - 2];
                triangle[i][k] = val;
            }
        }
    }
    triangle_rows = (size_t)(upto + 1);
}

/*--------------------------------------------------------------------
 *  Sequence entry function
 *--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2   */
    double d = sqrt(8.0 * (double)n + 1.0);
    size_t r = (size_t)floor((d - 1.0) / 2.0);

    /* Compute the starting index of row r */
    idx_t start = (idx_t)r * (r + 1) / 2;
    idx_t k     = n - start;          /* position within the row   */

    /* Compute rows up to r if necessary */
    ensure_rows((idx_t)r);

    /* Return the required entry */
    return triangle[r][(size_t)k];
}

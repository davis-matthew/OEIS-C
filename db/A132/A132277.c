/*
 * A132277  Triangle read by rows: T(n,k) is number of paths in the first
 *          quadrant from (0,0) to (n,0) using steps U=(1,1), D=(1,-1),
 *          h=(1,0) and H=(2,0), having exactly k h-steps.
 * https://oeis.org/A132277
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 37, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A132277: flattened triangle of lattice‑path counts.
 *  T(n,k) counts paths from (0,0) to (n,0) in the first quadrant using
 *  steps U=(1,1), D=(1,-1), h=(1,0) and H=(2,0) with exactly k h‑steps.
 *  a(n) = T(row, col) where n = row·(row+1)/2 + col   (0 ≤ col ≤ row).
 *
 *  The generating function for the polynomials
 *      C_n(t) = Σ_k T(n,k) t^k
 *  satisfies C_0 = 1 and, for n ≥ 1,
 *
 *      C_n = t · C_{n-1} + C_{n-2} + Σ_{i=0}^{n-2} C_i · C_{n-2-i}.
 *
 *  The implementation builds these polynomials on demand and returns
 *  the requested coefficient.  All arithmetic uses the seq_t type,
 *  which may be any unsigned integer type (including an arbitrary‑
 *  precision class).  No floating point or external libraries are used.
 */

#include <stdlib.h>   /* malloc, calloc, realloc */

/* forward declaration of the sequence function */
seq_t a(idx_t n);

/* ---------- dynamic storage for the polynomial rows ---------- */
static seq_t **poly = NULL;      /* poly[i] points to array of coeffs for C_i(t) */
static size_t *poly_len = NULL;  /* length of each polynomial (row+1)            */
static size_t poly_cap = 0;      /* allocated capacity of poly[]                 */
static size_t poly_count = 0;    /* number of rows currently stored              */

/* ------------------------------------------------------------------ */
/* Find the triangle row containing the flat index 'flat' (0‑based).   */
/* Uses integer arithmetic only; __int128 is for intermediate range. */
static idx_t
row_of(idx_t flat)
{
    idx_t low = 0;
    idx_t high = 1;
    /* Exponentially grow high until  high*(high+1)/2 > flat  */
    while ((unsigned __int128)high * (high + 1) / 2 <= flat) {
        high <<= 1;
    }
    /* Binary search between low and high */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 <= flat) {
            low = mid;
        } else {
            high = mid;
        }
    }
    return low;
}

/* ------------------------------------------------------------------ */
/* Ensure that all polynomial rows up to 'upto' are computed and
 * stored.  New rows are added to the dynamic arrays as needed.      */
static void
ensure_polys(idx_t upto)
{
    /* Initial allocation if this is the first call */
    if (poly == NULL) {
        poly_cap = 4;
        poly = (seq_t **)malloc(poly_cap * sizeof(seq_t *));
        poly_len = (size_t *)malloc(poly_cap * sizeof(size_t));

        /* Row 0: C_0(t) = 1 */
        poly[0] = (seq_t *)malloc(sizeof(seq_t));
        poly[0][0] = (seq_t)1;
        poly_len[0] = 1;
        poly_count = 1;
    }

    /* Grow capacity if necessary */
    while (upto >= poly_cap) {
        poly_cap <<= 1;
        poly = (seq_t **)realloc(poly, poly_cap * sizeof(seq_t *));
        poly_len = (size_t *)realloc(poly_len, poly_cap * sizeof(size_t));
    }

    /* Compute missing rows, one at a time */
    for (size_t cur = poly_count; cur <= (size_t)upto; ++cur) {
        size_t cur_len = cur + 1;
        seq_t *cur_poly = (seq_t *)calloc(cur_len, sizeof(seq_t));
        poly[cur] = cur_poly;
        poly_len[cur] = cur_len;

        /* t * C_{cur-1}  -> shift by one degree */
        for (size_t i = 0; i < poly_len[cur - 1]; ++i) {
            cur_poly[i + 1] += poly[cur - 1][i];
        }

        /* + C_{cur-2} one row lower (if it exists)   */
        if (cur >= 2) {
            for (size_t i = 0; i < poly_len[cur - 2]; ++i) {
                cur_poly[i] += poly[cur - 2][i];
            }

            /* + Σ_{i=0}^{cur-2} C_i * C_{cur-2-i}  (polynomial convolution) */
            for (size_t i = 0; i + 2 <= cur; ++i) {
                seq_t *A = poly[i];
                seq_t *B = poly[cur - 2 - i];
                size_t a_len = poly_len[i];
                size_t b_len = poly_len[cur - 2 - i];
                for (size_t a = 0; a < a_len; ++a) {
                    if (A[a] == 0) continue;
                    for (size_t b = 0; b < b_len; ++b) {
                        if (B[b] == 0) continue;
                        cur_poly[a + b] += A[a] * B[b];
                    }
                }
            }
        }
    }

    poly_count = upto + 1;
}

/* ------------------------------------------------------------------ */
/* The public sequence function.  Returns the flattened value a(n).    */
seq_t
a(idx_t n)
{
    /* Determine the triangle row and column for the flat index n */
    idx_t row = row_of(n);
    idx_t offset = (idx_t)((unsigned __int128)row * (row + 1) / 2);
    idx_t k = n - offset;

    /* Ensure the required polynomial row is computed */
    ensure_polys(row);

    /* k is guaranteed to be within [0, row] */
    return poly[row][k];
}

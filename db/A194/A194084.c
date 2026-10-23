/*
 * A194084  Triangle read by rows: a(n)=A135929(n) + A192011(n). Row n gives
 *          coefficients of polynomials BC(n,x) in order of decreasing
 *          exponents.
 * https://oeis.org/A194084
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 54, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:  BC(0)=0, BC(1)=3x, BC(2)=3x^2+3, BC(n)=x·BC(n‑1)–BC(n‑2) for n>2.
   a(n) is the n‑th element of the triangle obtained by writing the
   coefficients of BC(r) for r=0,1,2,… in order of decreasing powers. */

/* Headers */
#include <stdlib.h>
#include <stdint.h>

/* Allocate array of seq_t (works for C and C++) */
#if defined(__cplusplus)
#  define ALLOC(len)  new seq_t[(len)]
#else
#  define ALLOC(len)  ((seq_t*)malloc((len) * sizeof(seq_t)))
#endif

/* Maximum number of rows we keep (sufficient for n up to ~1e5). */
#define MAX_ROWS 20000

/* Static storage for rows of coefficients */
static seq_t *rows[MAX_ROWS] = {0};
static size_t row_len[MAX_ROWS] = {0};

/* Highest row index that has already been generated. */
static idx_t max_row = 0;

/* Find the row r such that the global index n is in row r
   (row r has r+1 coefficients).  */
static inline idx_t find_row(idx_t n) {
    idx_t r = 0;
    __uint128_t tri = 0;
    while (tri <= n) {
        r++;
        tri = (__uint128_t)r * (r + 1) / 2;   /* T(r) = number of elements up to row r-1 */
    }
    return r - 1;   /* previous row contains n */
}

/* Ensure that all rows up to r are computed.  */
static void ensure_rows(idx_t r) {
    /* Initial base rows (needed for all n).  */
    if (max_row == 0) {
        /* Row 0: BC(0) = 0 */
        rows[0] = ALLOC(1);
        rows[0][0] = (seq_t)0;
        row_len[0] = 1;

        /* Row 1: BC(1) = 3x */
        rows[1] = ALLOC(2);
        rows[1][0] = (seq_t)3;   /* coeff of x^1 */
        rows[1][1] = (seq_t)0;   /* constant term */
        row_len[1] = 2;

        /* Row 2: BC(2) = 3x^2 + 3 */
        rows[2] = ALLOC(3);
        rows[2][0] = (seq_t)3;   /* coeff of x^2 */
        rows[2][1] = (seq_t)0;   /* coeff of x^1 */
        rows[2][2] = (seq_t)3;   /* constant term */
        row_len[2] = 3;

        max_row = 2;
    }

    /* If the requested row is already available, nothing more to do. */
    if (r <= max_row) return;

    /* Guard against exceeding the preallocated storage. */
    if (r >= MAX_ROWS) return;

    /* Generate rows from max_row+1 up to r (recurrence applies for k≥3). */
    for (idx_t k = max_row + 1; k <= r; ++k) {
        size_t len = k + 1;
        seq_t *ptr = ALLOC(len);

        /* x · BC(k‑1) → shift coefficients left, append zero at the end. */
        for (idx_t i = 0; i < k; ++i) ptr[i] = rows[k - 1][i];
        ptr[k] = (seq_t)0;

        /* Subtract BC(k‑2) aligned with exponents. */
        if (k >= 3) {
            for (idx_t i = 0; i < k - 1; ++i)
                ptr[2 + i] -= rows[k - 2][i];
        }

        rows[k] = ptr;
        row_len[k] = len;
    }
    max_row = r;
}

/* Public sequence function requested by the harness. */
seq_t a(idx_t n)
{
    idx_t r = find_row(n);          /* row containing n */
    ensure_rows(r);                 /* make sure rows up to r are generated */

    /* Global start index of row r: T(r) = r*(r+1)/2. */
    idx_t start = (idx_t)(((__uint128_t)r * (r + 1)) / 2);

    idx_t pos = n - start;          /* position within the row (0‑based) */
    return rows[r][pos];            /* coefficient of x^(r−pos) in BC(r,x) */
}

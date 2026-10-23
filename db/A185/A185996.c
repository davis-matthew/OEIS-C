/*
 * A185996  Coefficient array of orthogonal polynomials
 *          P(n,x)=(x-2n+2)*P(n-1,x)-(2n-3)*P(n-2,x), P(0,x)=1, P(1,x)=x-1.
 * https://oeis.org/A185996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, tabl
 * Formula:   not stated
 * Range:     i8 n <= 15, i16 n <= 29, i32 n <= 65, i64 n <= 65, i128 n <= 65,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A185996: Coefficient array of orthogonal polynomials
   P(0,x)=1, P(1,x)=x-1,
   P(n,x) = (x-2n+2)*P(n-1,x) - (2n-3)*P(n-2,x)
   The sequence lists the coefficients of P(0),P(1),P(2),… in order. */

/* External integer types supplied by the build */
#include <stdlib.h>

/* Static storage for coefficient rows
   coeff_rows[n] points to an array of n+1 seq_t elements,
   coefficient of x^i is coeff_rows[n][i]; 0 ≤ i ≤ n. */
static seq_t **coeff_rows = NULL;  /* null until first use */
static idx_t  max_poly   = 0;      /* highest n for which coeff_rows[n] is valid */

/* Ensure that coeff_rows[0]..coeff_rows[n] exist.
   Rows are filled using the recurrence and stored for future calls. */
static void
ensure_polys_up_to(idx_t n)
{
    /* Initialise the first row if needed */
    if (coeff_rows == NULL) {
        coeff_rows = (seq_t**)calloc(1, sizeof(seq_t*));
        coeff_rows[0] = (seq_t*)calloc(1, sizeof(seq_t));
        coeff_rows[0][0] = (seq_t)1;   /* P(0,x) = 1 */
        max_poly = 0;
    }

    if (max_poly >= n)
        return;   /* already computed enough */

    /* Re‑allocate the array of pointers to hold rows up to n */
    coeff_rows = (seq_t**)realloc(coeff_rows,
                                  (size_t)(n + 1) * sizeof(seq_t*));

    /* Compute new rows in increasing order */
    for (idx_t m = max_poly + 1; m <= n; ++m) {
        /* Allocate array for coefficients of P(m,x) */
        seq_t *row = (seq_t*)calloc((size_t)(m + 1), sizeof(seq_t));

        if (m == 0) {                /* base case already set, keep as 1 */
            row[0] = (seq_t)1;
        } else if (m == 1) {         /* P(1,x) = x - 1 */
            row[0] = (seq_t)-1;
            row[1] = (seq_t)1;
        } else {
            /* Use the recurrence:
               P(m) = (x - (2m-2))*P(m-1) - (2m-3)*P(m-2)    */
            seq_t *prev = coeff_rows[m - 1];
            seq_t *prev2 = coeff_rows[m - 2];
            seq_t *cur   = row;

            seq_t a = (seq_t)((seq_t)2 * (seq_t)m - (seq_t)2);    /* 2m-2 */
            seq_t b = (seq_t)((seq_t)2 * (seq_t)m - (seq_t)3);    /* 2m-3 */

            /* (x - a) * P(m-1) */
            for (idx_t i = 0; i < m; ++i) {
                cur[i + 1] += prev[i];        /* multiply by x (shift) */
                cur[i] -= a * prev[i];        /* subtract a * prev[i] */
            }

            /* subtract b * P(m-2) */
            for (idx_t i = 0; i < m - 1; ++i)
                cur[i] -= b * prev2[i];
        }

        coeff_rows[m] = row;
    }

    max_poly = n;
}

/* Public function: returns the n-th term of the flattened coefficient array. */
seq_t
a(idx_t n)
{
    /* Find the polynomial index N such that N*(N+1)/2 ≤ n < (N+1)*(N+2)/2. */
    idx_t N = 0;
    while ((N + 1) * (N + 2) / 2 <= n)
        ++N;

    /* Index of the first coefficient of polynomial N in the flattened array. */
    idx_t start = N * (N + 1) / 2;
    idx_t k = n - start;   /* position within P(N) (0‑based) */

    ensure_polys_up_to(N);
    return coeff_rows[N][k];
}

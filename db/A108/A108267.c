/*
 * A108267  Triangle read by rows, T(n, k) = [x^k] (1-x)^(n+1)*Sum_{j=0..n}
 *          binomial(n + n*j + j, n*j + j)*x^j.
 * https://oeis.org/A108267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   binom_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A108267 – triangle read by rows.
   T(n,k) = Sum_{i=0..k} (-1)^i * binomial(n+1,i) * binomial(n + (n+1)*(k-i), n)
   a(n) is the element obtained by writing all T(r,c) row‑by‑row.
   Indexing follows OEIS: a(0)=T(0,0), a(1)=T(1,0), a(2)=T(1,1), ...
*/

#include <stdlib.h>
#include "../utils/binom_val.h"   /* provides seq_t binom_val(idx_t N, idx_t K) */

static seq_t **triangle = NULL;     /* pointers to the rows of the triangle   */
static idx_t   tra_rows = 0;        /* number of rows currently stored       */

/* make sure rows 0 .. r are available */
static void ensure_row(idx_t r)
{
    if (r < tra_rows) {
        return; /* already computed */
    }

    /* enlarge the array of row pointers if needed */
    if (tra_rows == 0) {
        triangle = (seq_t **)malloc((r + 1) * sizeof(seq_t *));
    } else if (r + 1 > tra_rows) {
        triangle = (seq_t **)realloc(triangle, (r + 1) * sizeof(seq_t *));
    }

    /* compute each new row */
    for (idx_t i = tra_rows; i <= r; ++i) {
        /* allocate the row (i+1) elements, initialised to 0 */
        triangle[i] = (seq_t *)calloc(i + 1, sizeof(seq_t));

        /* first and last entries are 1 */
        triangle[i][0] = 1;
        if (i > 0) {
            triangle[i][i] = 1;
        }

        /* middle entries */
        idx_t half = i / 2;
        for (idx_t k = 1; k <= half; ++k) {
            seq_t sum = 0;
            for (idx_t j = 0; j <= k; ++j) {
                seq_t b1 = binom_val(i + 1, j);
                idx_t N  = i + (i + 1) * (k - j);
                seq_t b2 = binom_val(N, i);
                seq_t term = b1 * b2;
                if (j & 1) {
                    sum -= term;           /* subtract for odd j */
                } else {
                    sum += term;           /* add for even j */
                }
            }
            triangle[i][k] = sum;
            /* use symmetry for the mirrored position if distinct */
            if (k != i - k) {
                triangle[i][i - k] = sum;
            }
        }
    }
    tra_rows = r + 1;
}

/* public entry point – returns a(n) */
seq_t a(idx_t n)
{
    /* find row r and column k such that n = r(r+1)/2 + k   */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }
    idx_t k = n - r * (r + 1) / 2;

    ensure_row(r);
    return triangle[r][k];
}

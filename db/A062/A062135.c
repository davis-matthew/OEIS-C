/*
 * A062135  Odd-numbered columns of Losanitsch triangle A034851 formatted as
 *          triangle with an additional first column.
 * https://oeis.org/A062135
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = T(r,m) where (r,m) are the row and column of the
 *            lower‑triangular array T defined by Losanitsch’s triangle
 *            A034851. For a given n, r = floor((sqrt(8n+1)-1)/2) and m = n -
 *            r(r+1)/2. T is computed by the recurrence: T(0,0)=1; T(n,0)=0
 *            (n>=1); for n>=1, m>=1: T(n,m) = T(n-1,m) + Σ_{k=m-1}^{n-...
 * Range:     u8 n <= 48, u16 n <= 125, u32 n <= 387, u64 n <= 1295,
 *            u128 n <= 4791, big any n
 * Helpers:   alloc_seq_array, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/binomial.h"
#include "../utils/alloc_seq_array.h"

static seq_t **triangle = NULL;           /* triangular array storage */
static size_t rows_capacity = 0;          /* allocated capacity for row pointers */
static size_t rows_count = 0;             /* number of rows that have been computed */

/* Ensure that rows 0..needed are available, computing any missing ones. */
static void ensure_rows(size_t needed)
{
    if (needed < rows_capacity) {
        /* capacity already large enough */
    } else {
        size_t new_cap = rows_capacity ? rows_capacity : 1;
        while (new_cap <= needed) new_cap <<= 1;
        seq_t **tmp = (seq_t**)realloc(triangle, new_cap * sizeof(seq_t*));
        triangle = tmp;
        rows_capacity = new_cap;
    }

    if (rows_count <= needed) {
        for (size_t r = rows_count; r <= needed; ++r) {
            seq_t *row = alloc_seq_array(r + 1);   /* allocate row of length r+1 */
            /* initialize all entries to 0 */
            for (size_t j = 0; j <= r; ++j) row[j] = 0;
            if (r == 0) {
                row[0] = 1;                         /* T(0,0) = 1 */
            } else {
                row[0] = 0;                         /* T(r,0) = 0 for r>=1 */
                for (size_t m = 1; m <= r; ++m) {
                    seq_t val = 0;
                    /* T(r-1,m) if defined */
                    if (m <= r - 1) val = triangle[r - 1][m];
                    /* sum_{k=m-1}^{r-1} T(k,m-1) */
                    for (size_t k = m - 1; k <= r - 1; ++k) {
                        val += triangle[k][m - 1];
                    }
                    /* odd n+m: subtract binomial term */
                    if (((r + m) & 1) == 1) {
                        idx_t num = (idx_t)(r + m - 3) / 2;
                        idx_t den = (idx_t)(m - 1);
                        val -= binomial(num, den);
                    }
                    row[m] = val;
                }
            }
            triangle[r] = row;
        }
        rows_count = needed + 1;
    }
}

seq_t a(idx_t n)
{
    /* Find row r such that r(r+1)/2 <= n < (r+1)(r+2)/2   (row lengths are r+1) */
    size_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;
    size_t m = (size_t)(n - r * (r + 1) / 2);

    ensure_rows(r);
    return triangle[r][m];
}

/*
 * A217537  Triangle read by rows, T(n,k) = T(n-1,k-1) + k*T(n-1,k) +
 *          (k+1)*T(n-1,k+1), T(0,0) = 1, n >= 0, k >= 0.
 * https://oeis.org/A217537
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 56, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle read by rows:
   T(n,k) = T(n-1,k-1) + k*T(n-1,k) + (k+1)*T(n-1,k+1),  T(0,0)=1.
   a(n) is the nth element of the flattened triangle (n≥1). */
#include <stdlib.h>
#include <stddef.h>

/* Global cache for triangle rows.  triangle[i] points to an array of i+1
   values T(i,0..i).  max_row records the last fully computed row index. */
static seq_t **triangle = NULL;
static idx_t max_row = (idx_t)-1;

/* Ensure that rows 0..up_to are computed and stored in triangle[]. */
static void ensure_rows(idx_t up_to)
{
    if (triangle == NULL) {
        triangle = (seq_t **)calloc((size_t)(up_to + 1), sizeof(seq_t *));
    } else if (up_to > max_row) {
        triangle = (seq_t **)realloc((void *)triangle,
                                     (size_t)(up_to + 1) * sizeof(seq_t *));
    }
    /* Allocate and compute any rows that are missing. */
    for (idx_t i = 0; i <= up_to; ++i) {
        if (triangle[i] != NULL)
            continue;                /* row already built */
        triangle[i] = (seq_t *)calloc((size_t)(i + 1), sizeof(seq_t));
        for (idx_t k = 0; k <= i; ++k) {
            seq_t v = 0;
            if (i == 0 && k == 0) {
                v = (seq_t)1;              /* base case */
            } else {
                if (k >= 1)
                    v += triangle[i - 1][k - 1];
                if (k <= i - 1)
                    v += (seq_t)k * triangle[i - 1][k];
                if (k + 1 <= i - 1)
                    v += (seq_t)(k + 1) * triangle[i - 1][k + 1];
            }
            triangle[i][k] = v;
        }
    }
    max_row = up_to;
}

/* Return the nth term of A217537 (offset 1). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* offset 1; this case should not occur */

    /* Find the row `r` such that the first r+1 rows contain at least n items. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 < n)
        ++r;

    idx_t prev = r * (r + 1) / 2;           /* items before row r */
    idx_t k = n - prev - 1;                 /* column within row r (0‑based) */

    ensure_rows(r);
    return triangle[r][k];
}

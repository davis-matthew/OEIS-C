/*
 * A227450  Triangular array read by rows. T(n,k) = A008277(n,k)*2^k; n >= 1,
 *          1 <= k <= n.
 * https://oeis.org/A227450
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = value of the triangular array T(n,k) = S(n,k)*2^k, read
 *            by rows where S(n,k) are the Stirling numbers of the second kind
 *            (A008277)
 * Range:     u8 n <= 17, u16 n <= 39, u32 n <= 97, u64 n <= 283,
 *            u128 n <= 793, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/pow2.h"          /* seq_t pow2(idx_t) */

static seq_t **triangle = NULL;   /* triangle[r][k] = S(r,k) for r>=1, 1<=k<=r */
static idx_t max_row = 0;          /* largest row whose values are stored */

/* Compute a single Stirling entry row r (r>=1) and store it in triangle[r] */
static void compute_row(idx_t r)
{
    seq_t *row = (seq_t *)malloc((size_t)(r + 1) * sizeof(seq_t));
    if (!row) return;                          /* failure – caller will abort on use */

    row[0] = 0;                                 /* unused entry */

    if (r == 1) {
        row[1] = 1;                             /* S(1,1) = 1 */
    } else {
        seq_t *prev = triangle[r - 1];
        for (idx_t k = 1; k < r; ++k) {
            /* S(r,k) = k*S(r-1,k) + S(r-1,k-1) */
            row[k] = ((seq_t)k * prev[k]) + prev[k - 1];
        }
        row[r] = 1;                             /* S(r,r) = 1 */
    }

    triangle[r] = row;
    if (r > max_row) max_row = r;
}

/* Extend the triangle array so that rows 1..r are available */
static void ensure_rows(idx_t r)
{
    if (r <= max_row) return;

    size_t new_cap = (size_t)(r + 1);
    triangle = (seq_t **)realloc(triangle, new_cap * sizeof(seq_t *));
    if (!triangle) return;                      /* failure – caller will abort on use */

    idx_t start = max_row;
    for (idx_t m = start + 1; m <= r; ++m) {
        compute_row(m);
    }
}

/* Return the nth term (1-indexed) of the OEIS sequence A227450. */
seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* sequence is defined for n ≥ 1 */

    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 < n) ++r;

    ensure_rows(r);

    /* Column k = n - (r-1)*r/2 within row r */
    idx_t k = n - ((r - 1) * r) / 2;

    seq_t stirling = triangle[r][k];
    seq_t pow2k   = pow2(k);     /* helper: returns 2^k as seq_t */

    return stirling * pow2k;
}

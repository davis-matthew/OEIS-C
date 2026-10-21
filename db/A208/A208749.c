/*
 * A208749  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A208750; see the Formula section.
 * https://oeis.org/A208749
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   T(0,0)=1; T(1,0)=1; for n ≥ 2, 0 ≤ k < n * T(n,k) = 2*T(n-1,k) –
 *            T(n-2,k) + 2*T(n-2,k-1) + 2*T(n-2,k-2), * with T(n,k)=0 when k<0
 *            or k≥n. * * Sequence a(n) is obtained by reading the triangle
 *            row by row * (starting with row 1) and listing its entries. Thus
 *            * a(1)=T(1,0), a(2)=T(2,0), a(3)=T(2,1),...
 * Range:     u8 n <= 24, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The smallest r such that r*(r+1)/2 ≥ n.  r≥1. */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < n)
        ++r;

    /* Column index within the r‑th row (0‑based). */
    seq_t tprev = (seq_t)(r - 1) * r / 2;
    size_t col = (size_t)(n - tprev - 1);

    /* Row 0. */
    seq_t *row0 = (seq_t *)malloc(sizeof(seq_t));
    if (!row0) return 0;
    row0[0] = (seq_t)1;
    size_t len0 = 1;

    /* Row 1. */
    if (r == 1) {
        seq_t val = row0[0];   /* a(1) = 1 */
        free(row0);
        return val;
    }

    seq_t *row1 = (seq_t *)malloc(sizeof(seq_t));
    if (!row1) { free(row0); return 0; }
    row1[0] = (seq_t)1;            /* T(1,0)=1 */
    size_t len1 = 1;

    /* If r==2 we can answer directly. */
    if (r == 2) {
        seq_t val = (col == 0) ? row1[0] : (seq_t)2;
        free(row0); free(row1);
        return val;
    }

    /* Compute rows 2 … r, keeping only the last two rows at a time. */
    seq_t *prev2 = row0;   /* T(n-2,·) */
    size_t len_prev2 = len0;
    seq_t *prev1 = row1;   /* T(n-1,·) */
    size_t len_prev1 = len1;
    seq_t *curr  = NULL;
    size_t len_curr = 0;

    for (idx_t i = 2; i <= r; ++i) {
        len_curr = (size_t)i;
        curr = (seq_t *)malloc(len_curr * sizeof(seq_t));
        if (!curr) {
            free(prev2);
            free(prev1);
            return 0;
        }
        for (size_t k = 0; k < len_curr; ++k) {
            seq_t t1 = 0, t2 = 0, t3 = 0, t4 = 0;
            if (k < len_prev1)
                t1 = 2 * prev1[k];
            if (k < len_prev2)
                t4 = prev2[k];
            if (k > 0 && (k - 1) < len_prev2)
                t2 = 2 * prev2[k - 1];
            if (k > 1 && (k - 2) < len_prev2)
                t3 = 2 * prev2[k - 2];
            curr[k] = t1 - t4 + t2 + t3;
        }
        /* Rotate rows: prev2←prev1, prev1←curr. */
        free(prev2);
        prev2 = prev1;
        len_prev2 = len_prev1;
        prev1 = curr;
        len_prev1 = len_curr;
    }

    /* We now have the r‑th row in prev1. */
    seq_t result = prev1[col];
    free(prev2);
    free(prev1);
    return result;
}

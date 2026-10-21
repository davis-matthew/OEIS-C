/*
 * A209139  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A209140; see the Formula section.
 * https://oeis.org/A209139
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 23, u16 n <= 53, u32 n <= 53, u64 n <= 53, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Triangle of coefficients of polynomials u(n,x) (OEIS A209139)
   Recurrence for T(n,k) (0 ≤ k ≤ n-1):
     T(n,k) = T(n‑1,k) + 2*T(n‑1,k‑1) + T(n‑2,k) + T(n‑2,k‑2)
   with initial rows
     T(1,0) = 1
     T(2,0) = 2,  T(2,1) = 1
   a(n) is obtained by reading the triangle row‑by‑row:
     r = smallest integer with r(r+1)/2 ≥ n
     k = n – r(r‑1)/2 – 1   (0‑based index within row r)
   The function returns T(r,k).
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* undefined; safety guard */
    if (n == 1) return (seq_t)1;

    /* find row r such that r(r+1)/2 ≥ n */
    idx_t r = 1;
    idx_t cum = 1;
    while (cum < n) {
        ++r;
        cum += r;
    }
    /* 0‑based index within the row */
    idx_t pos = n - (cum - r) - 1;

    if (r == 2) {                  /* rows 1 and 2 are known */
        return (pos == 0) ? (seq_t)2 : (seq_t)1;
    }

    /* allocate the first two rows */
    seq_t *row_prevprev = (seq_t*)malloc(sizeof(seq_t));
    if (!row_prevprev) return (seq_t)0;
    row_prevprev[0] = (seq_t)1;

    seq_t *row_prev = (seq_t*)calloc(2, sizeof(seq_t));
    if (!row_prev) { free(row_prevprev); return (seq_t)0; }
    row_prev[0] = (seq_t)2;
    row_prev[1] = (seq_t)1;

    /* build rows 3 … r */
    for (idx_t cur = 3; cur <= r; ++cur) {
        size_t sz = (size_t)cur;
        seq_t *row_cur = (seq_t*)calloc(sz, sizeof(seq_t));
        if (!row_cur) {
            free(row_prevprev);
            free(row_prev);
            return (seq_t)0;
        }
        for (idx_t k = 0; k < cur; ++k) {
            seq_t val = (seq_t)0;
            if (k < cur-1)          val += row_prev[(size_t)k];
            if (k > 0)              val += (seq_t)2 * row_prev[(size_t)(k-1)];
            if (k < cur-2)          val += row_prevprev[(size_t)k];
            if (k >= 2)             val += row_prevprev[(size_t)(k-2)];
            row_cur[(size_t)k] = val;
        }
        free(row_prevprev);
        row_prevprev = row_prev;
        row_prev = row_cur;
    }

    seq_t result = row_prev[(size_t)pos];
    free(row_prevprev);
    free(row_prev);
    return result;
}

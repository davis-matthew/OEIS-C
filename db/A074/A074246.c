/*
 * A074246  Triangle of coefficients, read by rows, where the n-th row forms
 *          the polynomial P(n,x) = {Sum_{k=1..n} 1/(k+x)}*{Product_{k=1..n}
 *          (k+x)}.
 * https://oeis.org/A074246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nice, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 78, u64 n <= 191,
 *            u128 n <= 561, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A074246
 *
 * a(n) is the (n‑th) coefficient read by rows of the triangle whose
 * r‑th row is the polynomial
 *     P(r,x) = (∑_{i=1..r} 1/(i+x)) * ∏_{i=1..r} (i+x)
 *
 *  a(n) = k * C(r+1, k+1)
 *
 *  where r ≥ 1 is the largest integer with       r(r+1)/2 < n  (i.e. the
 *  row containing the n-th term) and  
 *        k = n − r(r−1)/2   (position in that row, 1 ≤ k ≤ r).
 *
 *  C(n,k) is the unsigned Stirling number of the first kind.
 *  It satisfies
 *
 *          C(0,0) = 1,      C(n,0) = 0   (n > 0)
 *          C(n,k) = C(n‑1,k‑1) + (n‑1) * C(n‑1,k)    (n > 0, k > 0)
 *
 *  This implementation computes the required Stirling number by
 *  building the triangle up to row (r+1) using the recurrence above.
 *  The algorithm runs in O(r²) time and uses O(r) space, where
 *  r ≈ √(2n).  For n ≤ 10 000 (as required) the maximum r is 141,
 *  so the routine is very fast.
 */

#include <stdlib.h>
#include <stdint.h>

/* seq_t and idx_t are defined by the build environment.  */
seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;   /* undefined for n<1, fall back */

    /* --- find row r and position k ------------------------------------ */
    idx_t r   = 1;       /* current row */
    idx_t cum = 1;       /* cumulative number of terms up to row r */

    while (cum < n)
    {
        ++r;
        cum += r;
    }

    idx_t prev_cum = cum - r;   /* terms before row r */
    idx_t k        = n - prev_cum;   /* 1‑based index inside the row */

    /* --- compute unsigned Stirling number C(r+1, k+1) ------------------- */
    idx_t target_n = r + 1;      /* n in C(n,k) recurrence  */
    idx_t target_k = k + 1;

    size_t size = (size_t)target_n + 1;
    seq_t *c = (seq_t *)malloc(size * sizeof(seq_t));
    if (!c) return (seq_t)0;     /* memory failure – return 0 (unlikely) */

    for (size_t i = 0; i < size; ++i) c[i] = (seq_t)0;
    c[0] = (seq_t)1;             /* C(0,0) = 1 */

    for (idx_t row = 1; row <= target_n; ++row)
    {
        /* update in reverse so that previous values are not overwritten */
        for (idx_t i = row; i > 0; --i)
            c[i] = c[i-1] + (seq_t)(row-1) * c[i];
        c[0] = (seq_t)0;       /* C(row,0) = 0 for row>0 */
    }

    seq_t coeff = (seq_t)k * c[target_k];

    free(c);
    return coeff;
}

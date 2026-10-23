/*
 * A188843  T(n,k) is the number of n X k binary arrays without the pattern 0
 *          1 diagonally or vertically.
 * https://oeis.org/A188843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 68, u32 n <= 233, u64 n <= 746,
 *            u128 n <= 1741, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A188843 – number of binary n×k arrays avoiding 01
    diagonally or vertically.  a(n) is obtained by read‑off
    the table T(i,j) on its antidiagonals:
    a(1)=T(1,1), a(2)=T(1,2), a(3)=T(2,1), a(4)=T(1,3), ...
    Recurrence for T(i,k) (i≥1,k≥1):
        T(i,0) = 1
        T(i,k) = Σ_{t=1}^{⌊(i+2)/2⌋} (-1)^{t-1} · binom(i+2‑t,t) · T(i,k‑t)
    The helper “binom” is provided by ../utils/binom.h.  */
#include <stdint.h>
#include <stdlib.h>
#include "../utils/binom.h"      /* provides static seq_t binom(idx_t a, idx_t b) */

/* find smallest s such that (s-1)*s/2 ≥ n  (s is the diagonal index) */
static idx_t find_diag(idx_t n)
{
    idx_t s = 2;               /* first diagonal is s = 2 (cell 1,1) */
    idx_t cum = 1;             /* number of elements up to current diagonal */
    while (cum < n) {
        ++s;
        cum += s - 1;          /* add size of next diagonal (s-1) */
    }
    return s;
}

/* compute T(rows,cols) with the recurrence.  rows,cols ≥ 1.
   For cols==0 return 1 (though this case never occurs in a(n)). */
static seq_t compute_T(idx_t rows, idx_t cols)
{
    if (cols == 0) return 1;
    /* dynamic array for current row values T(rows,0..cols) */
    seq_t *row = (seq_t *)malloc((size_t)(cols + 1) * sizeof(seq_t));
    if (!row) return 0;                /* allocation failure – unlikely */
    row[0] = 1;                         /* T(rows,0) = 1 */

    idx_t max_t = (rows + 2) / 2;        /* floor((rows+2)/2) */

    for (idx_t k = 1; k <= cols; ++k) {
        seq_t pos = 0, neg = 0;         /* accumulate positive/negative parts */
        for (idx_t t = 1; t <= max_t; ++t) {
            if (k < t) break;           /* early termination */
            seq_t bin = binom(rows + 2 - t, t);
            seq_t prod = bin * row[k - t];
            if (t & 1ULL) pos += prod;   /* t odd → + sign   */
            else        neg += prod;     /* t even → – sign  */
        }
        row[k] = pos - neg;             /* pos ≥ neg for the recurrence  */
    }

    seq_t res = row[cols];
    free(row);
    return res;
}

/* external function required by the build system */
seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* not defined for n=0; return 0 as safe value */

    /* diagonal index s (i+j) that contains the n‑th cell */
    idx_t s = find_diag(n);

    /* number of elements before this diagonal   = (s-1)*(s-2)/2 */
    __uint128_t tmp = (__uint128_t)(s - 1) * (s - 2);
    idx_t prev = (idx_t)(tmp / 2);

    /* zero‑based offset inside the diagonal */
    idx_t offset = n - prev - 1;

    /* coordinates (i,j) corresponding to this offset */
    idx_t i = offset + 1;           /* 1 ≤ i ≤ s-1   */
    idx_t j = s - i;                /* 1 ≤ j ≤ s-1   */

    return compute_T(i, j);
}

/*
 * A123670  Triangle read by rows: T(n,k) is the coefficient of x^k of the
 *          polynomial n(n-x)(n-2x)(n-3x)...(n-(n-1)x) (n>=1, 0<=k<=n-1).
 * https://oeis.org/A123670
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   T(r,k) = (-1)^k * r^(r-k) * S(r, r-k) where S(r,m) is the
 *            unsigned Stirling number of the first kind. a(n) is the triangle
 *            read by rows, so a(n) = T(r,k) for the unique row r and column k
 *            given by the flattened index n
 * Range:     i8 n <= 6, i16 n <= 15, i32 n <= 38, i64 n <= 107, i128 n <= 307,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 1115 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* ... The function is fully O(r*k) per call, with r≤current row
   and k the column within that row. */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* unsigned Stirling numbers of the first kind: S(n,m)
   computed via the recurrence S(i,j)=S(i-1,j-1)+(i-1)*S(i-1,j) */
static seq_t stirling_unsigned(idx_t n, idx_t m) {
    size_t sz = (size_t)m + 1;
    seq_t *prev = mem_new(seq_t, sz);
    seq_t *cur  = mem_new(seq_t, sz);
    if (!prev || !cur) {
        mem_free(prev);
        mem_free(cur);
        return (seq_t)0;   /* allocation failed */
    }
    prev[0] = (seq_t)1;                /* S(0,0) = 1 */
    for (idx_t i = 1; i <= n; ++i) {
        size_t up = (i < m ? i : m);
        cur[0] = (seq_t)0;
        for (size_t j = 1; j <= up; ++j) {
            cur[j] = prev[j - 1] + ((seq_t)(i - 1)) * prev[j];
        }
        for (size_t j = up + 1; j <= m; ++j)
            cur[j] = (seq_t)0;
        /* swap buffers for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }
    seq_t res = prev[m];
    mem_free(prev);
    mem_free(cur);
    return res;
}

/* fast exponentiation for seq_t */
static seq_t pow_seq(seq_t base, idx_t exp) {
    seq_t result = (seq_t)1;
    while (exp) {
        if (exp & 1)
            result = result * base;
        base = base * base;
        exp >>= 1;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0)           /* offset starts at 1 */
        return (seq_t)0;

    /* Find row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    idx_t tri = (idx_t)1;          /* r*(r+1)/2 */
    while (tri < n) {
        ++r;
        tri = r * (r + 1) / 2;
    }
    idx_t prevtri = (r - 1) * r / 2;
    idx_t k = n - prevtri - 1;     /* 0‑based column */
    idx_t m = r - k;               /* r-k = n-k */

    /* Stirling number S(r, r-k) */
    seq_t s = stirling_unsigned(r, m);

    /* r^(r-k) */
    seq_t power = pow_seq((seq_t)r, (idx_t)(r - k));

    /* Combine with sign */
    seq_t coeff = s * power;
    if (k & 1)                     /* (-1)^k factor */
        coeff = -coeff;

    return coeff;
}

/*
 * A083861  Square array T(n,k) of second binomial transforms of generalized
 *          Fibonacci numbers, read by ascending antidiagonals, with n, k >=
 *          0.
 * https://oeis.org/A083861
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is obtained by reading the square array T(n,k) by ascending
 *            * antidiagonals. T satisfies T(.,0)=0 , T(.,1)=1 and for k>=2 *
 *            T(n,k) = 5*T(n,k-1) + (n-6)*T(n,k-2) . * The result of a(n) is
 *            T(i,j) where (i,j) is the (n‑th) entry in the * antidiagonal
 *            order
 * Range:     u8 n <= 25, u16 n <= 74, u32 n <= 206, u64 n <= 657,
 *            u128 n <= 2005, big any n
 * Validated: standard, pass, 82 terms through n = 4759 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static seq_t compute_T_rowk(idx_t row, idx_t k)
{
    if (k == 0) return (seq_t)0;
    if (k == 1) return (seq_t)1;

    seq_t prevPrev = (seq_t)0;   /* T(row,0) */
    seq_t prev     = (seq_t)1;   /* T(row,1) */
    for (idx_t t = 2; t <= k; ++t) {
        seq_t cur;
        if (row < 6) {
            int diff = 6 - (int)row;          /* positive */
            cur = prev * (seq_t)5 - (seq_t)diff * prevPrev;
        } else {
            int diff = (int)row - 6;          /* positive */
            cur = prev * (seq_t)5 + (seq_t)diff * prevPrev;
        }
        prevPrev = prev;
        prev     = cur;
    }
    return prev;
}

seq_t a(idx_t n)
{
    /* Find the antidiagonal m such that the n-th element lies in it.
     * S(m) = sum_{t=0}^{m} (t+1) = (m+1)(m+2)/2 is the number of
     * elements in antidiagonals 0..m.  We find the smallest m with
     * S(m) > n.  For n up to 10000, m < 200, so a simple loop is fast. */
    idx_t m = 0;
    idx_t cum = 1;        /* S(0) = 1 */
    while (cum <= n) {
        ++m;
        cum += m + 1;     /* add the length of antidiagonal m */
    }
    idx_t prev_cum = cum - (m + 1);   /* S(m-1) */
    idx_t offset   = n - prev_cum;    /* position inside diagonal m */
    idx_t i = m - offset;             /* row */
    idx_t j = offset;                 /* column */
    return compute_T_rowk(i, j);
}

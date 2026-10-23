/*
 * A173749  Square array T(n, k) = v(k, n)((1)), where v(n, q) = M*v(n-1, q),
 *          M = {{0, 1, 0}, {0, 0, 1}, {q^3, q^3, 0}}, with v(0, q) = {1, 1,
 *          1}, read by antidiagonals.
 * https://oeis.org/A173749
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth term of the square array T(n,k) read by
 *            antidiagonals, where T(n,k)=f(k,n+1) and f satisfies
 *            f(0)=f(1)=f(2)=1, f(k)= (n+1)³ · (f(k-2)+f(k-3)) for k≥3
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 109, u64 n <= 259,
 *            u128 n <= 673, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Integer square‑root of a 64‑bit unsigned number */
static inline idx_t isqrt_u64(idx_t x)
{
    idx_t lo = 0, hi = x + 1;          /* [lo,hi) is the search range */
    while (lo + 1 < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if (mid > 0 && mid > x / mid)  /* mid² > x  */
            hi = mid;
        else
            lo = mid;
    }
    return lo;
}

seq_t a(idx_t n)
{
    /* 1. Determine the diagonal d and the position pos within that diagonal */
    idx_t root = isqrt_u64(8 * n + 1);           /* floor(sqrt(8n+1))               */
    idx_t d    = (root - 1) / 2;                 /* current diagonal index          */
    idx_t start = d * (d + 1) / 2;               /* first index on this diagonal   */
    idx_t pos   = n - start;                     /* position within the diagonal   */
    idx_t q     = pos + 1;                       /* n' + 1  (parameter of the recurrence) */
    idx_t k     = d - pos;                       /* column index in T(n,k)         */

    /* 2. Compute f(k,q) using its recurrence; f(0)=f(1)=f(2)=1 */
    if (k <= 2)
        return (seq_t)1;

    seq_t Q   = (seq_t)q;
    seq_t q3  = Q * Q * Q;                       /* (n+1)³ */

    seq_t f_im3 = (seq_t)1; /* f(i-3)  */
    seq_t f_im2 = (seq_t)1; /* f(i-2)  */
    seq_t f_im1 = (seq_t)1; /* f(i-1)  */
    seq_t f_cur;

    for (idx_t i = 3; i <= k; ++i) {
        f_cur = q3 * (f_im2 + f_im3);           /* f(i) = q³·(f(i-2)+f(i-3)) */
        f_im3 = f_im2;
        f_im2 = f_im1;
        f_im1 = f_cur;
    }
    return f_cur;
}

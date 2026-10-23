/*
 * A196020  Irregular triangle read by rows: T(n,k), n >= 1, k >= 1, in which
 *          column k lists the odd numbers interleaved with k-1 zeros, and the
 *          first element of column k is in row k(k+1)/2.
 * https://oeis.org/A196020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(n, k) where k is the column index of the n‑th * entry
 *            in the triangle. The triangle rows are * T(m,k) = 2*[(m-
 *            k*(k+1)/2)/k + 1] - 1 if (m- k*(k+1)/2) % k == 0 * = 0 otherwise
 *            * Each row m has length L(m) = floor((sqrt(8*m+1)-1)/2)
 * Range:     u8 n <= 1255, u16 n <= 2489, u32 n <= 2489, u64 n <= 2489,
 *            u128 n <= 2489, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline idx_t
isqrt_u64(idx_t x)
{
    /* Simple integer square root for values that fit in idx_t
       (fast for small values such as 8*m+1). */
    idx_t r = 0;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

seq_t a(idx_t n)
{
    /* Indices in the OEIS start at 1.  For safety handle n==0. */
    if (n == 0) return (seq_t)0;

    idx_t cumulative = 0;   /* number of terms processed so far   */
    idx_t m = 1;            /* current row number                  */
    idx_t len;              /* number of columns in this row       */

    /* Find the row m that contains the n-th term. */
    for (;; ++m)
    {
        idx_t s = isqrt_u64(8 * m + 1);   /* floor(sqrt(8*m+1))          */
        len = (s - 1) / 2;                /* L(m)                         */
        if (n <= cumulative + len) break;/* n is inside this row        */
        cumulative += len;              /* move to next row             */
    }

    /* offset is the column index (1‑based) within row m. */
    idx_t offset = n - cumulative;     /* 1 .. len                      */
    idx_t k = offset;                  /* column number                 */
    idx_t startk = k * (k + 1) / 2;     /* first row of column k         */
    idx_t diff = m - startk;           /* distance from the first entry */

    if (diff % k == 0)
    {
        /* The entry is an odd number.  Calculate its position
           among the odds in this column. */
        idx_t odd_index = diff / k + 1;
        return (seq_t)2 * (seq_t)odd_index - 1;
    }
    else
    {
        /* The entry is the inserted zero. */
        return (seq_t)0;
    }
}

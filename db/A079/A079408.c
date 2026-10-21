/*
 * A079408  Array T(m,n) (m>=0, n>=0) read by antidiagonals: T(0, 0) = 1, T(0,
 *          n) = 0 if n != 0, T(m, n) = T(m-1, T(m-1, n)) + T(m-1, n - T(m-1,
 *          n-1)) if m > 0.
 * https://oeis.org/A079408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(m,ncol) where m and ncol are obtained from the
 *            antidiagonal ordering of the two‑dimensional array. For m == 0:
 *            1 if ncol == 0 else 0. For m == 1: 2 if ncol == 1 else 1. For m
 *            >= 2: general value = 3 * 2^(m-2). If ncol == general - 2 then
 *            a(n) = general - 1
 * Range:     u8 n <= 44, u16 n <= 79, u32 n <= 79, u64 n <= 79, u128 n <= 79,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The implementation uses the closed form described in the OEIS
   entry, avoiding any dynamic allocation or recursion. */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d such that d(d+1)/2 <= n < (d+1)(d+2)/2. */
    idx_t d = 0;
    while (((unsigned __int128)d * (d + 1)) / 2 <= n)
        d++;
    d--;                                      /* actual diagonal */

    idx_t before = d * (d + 1) / 2;            /* count before this diagonal */
    idx_t k = n - before;                     /* offset within diagonal */
    idx_t m = d - k;                          /* row index (m)  */
    idx_t ncol = k;                           /* column index (n) */

    /* Handle the values according to the closed form */
    if (m == 0) {
        return (ncol == 0) ? (seq_t)1 : (seq_t)0;
    } else if (m == 1) {
        return (ncol == 1) ? (seq_t)2 : (seq_t)1;
    } else {
        seq_t general = pow2((idx_t)(m - 2)) * (seq_t)3;  /* 3 * 2^(m-2) */
        seq_t special = general - (seq_t)2;              /* 3*2^(m-2) - 2 */
        return (ncol == special) ? (general - (seq_t)1) : general;
    }
}

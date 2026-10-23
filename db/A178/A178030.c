/*
 * A178030  Array read by antidiagonals: T(0,m)=2, T(1,m)=1, T(n,m)=A000032(n)
 *          and recursively T(n,m)=( T(n-1,m)^2 + (4*m + 1)*(-1)^n) / T(n-2,
 *          m), n>=0, m>=1.
 * https://oeis.org/A178030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 36, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   _floor_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A178030: a(n) = T(d - k, k+1)
   where d = floor((sqrt(8*n+1)-1)/2),
         k = n - d*(d+1)/2.  */
/* T(0,m)=2, T(1,m)=1,
   T(n,m) = (T(n-1,m)^2 + (4*m + 1)*(-1)^n) / T(n-2,m) for n>=2. */

#include <stddef.h>
#include "../utils/_floor_isqrt_u64.h"

/* compute T(r, c) for a given column c (m) and row r (n) */
static seq_t compute_T(idx_t row, idx_t col)
{
    /* C = 4*m + 1  */
    seq_t C = (seq_t)(4 * col + 1);

    if (row == 0) return (seq_t)2;
    if (row == 1) return (seq_t)1;

    seq_t prev_prev = (seq_t)2;          /* T(0, col) */
    seq_t prev = (seq_t)1;               /* T(1, col) */
    seq_t new_val;

    for (idx_t i = 2; i <= row; ++i) {
        seq_t numerator = prev * prev;   /* T(i-1)^2 */

        if (i & 1)                 /* (-1)^i == -1 for odd i */
            numerator -= C;
        else
            numerator += C;         /* (+1) for even i */

        new_val = numerator / prev_prev;

        prev_prev = prev;
        prev = new_val;
    }
    return prev;                     /* T(row, col) */
}

/* main sequence routine */
seq_t a(idx_t n)
{
    /* find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2 */
    idx_t d = (( _floor_isqrt_u64(8 * n + 1) - 1 ) >> 1);
    while (d * (d + 1) / 2 > n) d--;
    while ((d + 1) * (d + 2) / 2 <= n) d++;

    idx_t k = n - d * (d + 1) / 2;      /* offset inside diagonal */
    idx_t col = k + 1;                  /* column number m */
    idx_t row = d - k;                  /* row number n */

    return compute_T(row, col);
}

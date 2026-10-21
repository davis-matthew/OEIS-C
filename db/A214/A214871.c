/*
 * A214871  Natural numbers placed in table T(n,k) layer by layer. The order
 *          of placement - T(n,n), T(1,n), T(n,1), T(2,n), T(n,2),...T(n-1,n),
 *          T(n,n-1). Table T(n,k) read by antidiagonals.
 * https://oeis.org/A214871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = (i-1)^2+1 if i=j (i-1)^2+2*j+1 otherwise if i>j
 *            (j-1)^2+2*i if i<j where the pair (i,j) is on the (t+1)-th
 *            antidiagonal of T(n,k). t = floor( (-1+sqrt(8*n-7))/2 ) i = n -
 *            t*(t+1)/2 j = (t*t + 3*t + 4)/2 - n T(n,k) is defined by the
 *            given table formulas
 * Range:     u8 n <= 136, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute diagonal index t */
    long double d = sqrtl(8.0L * (long double)n - 7.0L);
    idx_t t = (idx_t)((d - 1.0L) / 2.0L);

    /* correct possible off‑by‑one due to rounding */
    while ( (__int128)(t + 1) * (t + 2) / 2 < n ) ++t;
    while ( (__int128)t * (t + 1) / 2 >= n ) --t;

    /* i and j on the diagonal */
    idx_t i = n - (__int128)t * (t + 1) / 2;
    idx_t j = ( (__int128)t * t + (__int128)3 * t + 4 ) / 2 - n;

    seq_t i_seq = (seq_t)i;
    seq_t j_seq = (seq_t)j;
    seq_t val;

    if (i == j) {
        val = (i_seq - 1) * (i_seq - 1) + 1;
    } else if (i > j) {
        val = (i_seq - 1) * (i_seq - 1) + 2 * j_seq + 1;
    } else {
        val = (j_seq - 1) * (j_seq - 1) + 2 * i_seq;
    }
    return val;
}

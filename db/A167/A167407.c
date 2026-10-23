/*
 * A167407  T(m,n) is -m if n=0, 1 elsewhere.
 * https://oeis.org/A167407
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 96, i16 n <= 96, i32 n <= 96, i64 n <= 96, i128 n <= 96,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangular enumeration: a(n) = -m if n is the first element
   of row m, otherwise 1.  m is the largest integer with
   m*(m+1)/2 <= n. */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case: n == 0 gives 0, consistent with T(0,0)=0 */
    if (n == 0) return 0;

    /* Find m such that m*(m+1)/2 <= n < (m+1)(m+2)/2.
       Use a binary search with 128‑bit arithmetic to avoid overflow. */
    idx_t low = 0, high = 1;
    while ( (unsigned __int128)high * (high + 1) / 2 <= n )
        high *= 2;

    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 val = (unsigned __int128)mid * (mid + 1) / 2;
        if (val <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t m = low - 1;

    /* Position within the row */
    idx_t offset = (unsigned __int128)m * (m + 1) / 2;
    idx_t k = n - offset;      /* k == 0 means first element of the row */

    if (k == 0) {
        seq_t signed_m = (seq_t)m;
        return -signed_m;
    } else {
        return (seq_t)1;
    }
}

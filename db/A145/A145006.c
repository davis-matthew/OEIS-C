/*
 * A145006  Triangle read by rows, generator for the partition numbers,
 *          A000041.
 * https://oeis.org/A145006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  eigen, tabl, sign
 * Formula:   a(0) = 1. For n > 0 write n as row·(row+1)/2 + col with 0 ≤ col
 *            ≤ row, where row is the unique integer satisfying row·(row+1)/2
 *            ≤ n < (row+1)·(row+2)/2. Let m = row – col. (m = 0 on the
 *            diagonal). If m is not a generalized pentagonal number, a(n) =
 *            0. If m = k·(3k‑1)/2 or k·(3k+1)/2 (k ≥ 1) then...
 * Range:     i8 n <= 77, i16 n <= 77, i32 n <= 77, i64 n <= 77, i128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* Integer square root of a 64‑bit value (floor) */
static idx_t isqrt_u64(idx_t x)
{
    idx_t low  = 0;
    idx_t high = (idx_t)1ULL << 32;          /* > sqrt(UINT64_MAX) */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        if (( (__int128)mid * mid ) <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

/* Return the signed value (+1, –1 or 0) for a generalized
   pentagonal number m (m>0).  If m is not such a number, 0 is returned. */
static seq_t pentagonal_sign(idx_t m)
{
    if (m == 0) return (seq_t)0;

    /* m is a generalized pentagonal number iff
       24*m+1 is a perfect square. */
    idx_t D = 24*m + 1;
    idx_t s = isqrt_u64(D);
    if (( (__int128)s * s ) != D) return (seq_t)0;

    /* k is the index in the pentagonal sequence:
         m = k*(3k-1)/2   or   m = k*(3k+1)/2. */
    idx_t k;
    if ((s + 1) % 6 == 0)
        k = (s + 1) / 6;      /* m = k*(3k-1)/2  (k > 0) */
    else                     /* s-1 % 6 == 0 because D is odd */
        k = (s - 1) / 6;      /* m = k*(3k+1)/2 */

    return (seq_t) ((k & 1) ? 1 : -1);
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;          /* a(0) */
    /* Find row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t low  = 0;
    idx_t high = 1;
    while ( ( (__int128)high * (high + 1) ) / 2 <= n )
        high <<= 1;                       /* exponential search */

/* Binary search */
    while (low < high) {
        idx_t mid = low + (high - low + 1) / 2;
        if ( ( (__int128)mid * (mid + 1) ) / 2 <= n )
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;
    idx_t prev = ( (__int128)r * (r + 1) ) / 2;
    idx_t c   = n - (idx_t)prev;
    idx_t m = r - c;                       /* m >= 0 */

    if (m == 0) return (seq_t)0;           /* diagonal entries (except a(0)) */
    return pentagonal_sign(m);
}
